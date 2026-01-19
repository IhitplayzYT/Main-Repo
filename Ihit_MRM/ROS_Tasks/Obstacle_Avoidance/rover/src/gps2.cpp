#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <sensor_msgs/msg/laser_scan.hpp>
#include <sensor_msgs/msg/imu.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <tf2/LinearMath/Quaternion.h>
#include <tf2/LinearMath/Matrix3x3.h>
#include <cmath>
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include <unistd.h>
#include <limits>
#include <vector>




using namespace std::chrono_literals;
int dealt = 0;
long double woah,target_lat_, target_lon_, current_lat_,current_lon_,set_lat,set_lon,x_mov,y_mov,distance,bearing,permb,permd,origin_lat_ = 0.00000,origin_lon_ = 0.00000;
int n = 0,m = -1;
double target_yaw;
int target_reached = 0;
double inf = std::numeric_limits<double>::infinity();
int flag = 0 ,flag2 = 0;
int obstacle_flag = 0;
double yaw,yaw_rad;
int q = 0;
auto temp = sensor_msgs::msg::NavSatFix::SharedPtr();
auto pemt = sensor_msgs::msg::LaserScan::SharedPtr();
int z = 0;
int y =0;
int dealing_obstacle = 0;
double mean = 0;
float least;
int stuck_counter;
int local_min = 0;
int min_index = -1;
float max_distance = -1;
int max_index = -1;



class GpsNavigator : public rclcpp::Node {
    public:
        GpsNavigator() : Node("gps")
        {if (z == 0){
            std::cout << "Input target latitude and longitude:" << std::endl;
                std::cin >> target_lat_;
                std::cin >> target_lon_;
            z++;}

                if (q == 0){
                m++;
                imu_subscriber_ = this->create_subscription<sensor_msgs::msg::Imu>("/imu", rclcpp::SensorDataQoS(), std::bind(&GpsNavigator::imuCallback, this, std::placeholders::_1));
                movement_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
                gps_sub_ = this->create_subscription<sensor_msgs::msg::NavSatFix>("/gps", rclcpp::SensorDataQoS(), std::bind(&GpsNavigator::gpsCallback, this, std::placeholders::_1));
                laser_sub = this->create_subscription<sensor_msgs::msg::LaserScan>("/scan", rclcpp::SensorDataQoS(), std::bind(&GpsNavigator::laserCallback, this, std::placeholders::_1));
                q++;
                }
    if (target_reached == 1)      
{rclcpp::shutdown();
exit(0);}
            

        }
    
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr movement_pub_;
        rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_sub_;
        rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscriber_;
        rclcpp::Subscription<sensor_msgs::msg::LaserScan>::SharedPtr laser_sub;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::TimerBase::SharedPtr timer_func;

        void laserCallback(const sensor_msgs::msg::LaserScan::SharedPtr msg) {
            float min_distance = std::numeric_limits<float>::max();
            
            pemt = msg;
        
            for (size_t i = 0; i < msg->ranges.size(); ++i) {
                if (msg->ranges[i] < min_distance && msg->ranges[i] > msg->range_min) {
                    min_distance = msg->ranges[i];
                    min_index = i;
                    if (msg->ranges[i] < 1.0)
                    {stuck_counter++;}
                }
                if (msg->ranges[i] > max_distance && msg->ranges[i] > msg->range_max)
                {max_distance = msg->ranges[i];
                max_index = i;
                }
        
            }
            least = min_distance;
                
            if (( (min_distance < 1.0) && ((((min_index > 0) && (min_index < 10)) || ((min_index > 350) && (min_index < 360))) || ((min_index >170) && (min_index < 190)))) || (stuck_counter > 5))
            {auto twist_msg = geometry_msgs::msg::Twist();
                twist_msg.angular.x = 0.0;
                twist_msg.angular.z = 0.0;
                movement_pub_->publish(twist_msg);
                dealing_obstacle = 1;
                escape_local_minimum(min_index,msg);
                
                }
            else if (min_distance < 1.5 && ((((min_index > 0) && (min_index < 10)) || ((min_index > 350) && (min_index < 360))) || ((min_index >170) && (min_index < 190)))) {
                auto twist_msg = geometry_msgs::msg::Twist();
                movement_pub_->publish(twist_msg);
                
                if (dealing_obstacle == 0)
                {
                twist_msg.angular.z = 0.0;
                twist_msg.linear.x = 0.0;}
                dealing_obstacle = 1;
                follow_boundary(min_index, msg);
                stuck_counter = 0;
            }
            else {
                if (y == 1)
                {auto twist_msg = geometry_msgs::msg::Twist();
                    twist_msg.angular.z = 0.0;
                    twist_msg.linear.x = 0.0;
                    movement_pub_->publish(twist_msg);
                    obstacle_flag = 1;
                    stuck_counter = 0;
                flag = 0;
                q = 0;
                dealing_obstacle = 0;
                gpsCallback(temp);}
                else
                {}
            }
            
        }
    
    
         void follow_boundary(int obstacle_index, const sensor_msgs::msg::LaserScan::SharedPtr msg) {
            auto cmd = geometry_msgs::msg::Twist();
            float angle_to_obstacle = msg->angle_min + obstacle_index * msg->angle_increment;            
            cmd.linear.x = -0.1;
            cmd.angular.z = (angle_to_obstacle > 0) ? -0.1 : 0.1;
            movement_pub_->publish(cmd);
            local_min = 1;
            y = 1;
            sleep(1);
            dealing_obstacle = 0;
            stuck_counter = 0;
            
        }


        void escape_local_minimum(int obstacle_index, const sensor_msgs::msg::LaserScan::SharedPtr msg) {
            float angle_to_obstacle = msg->angle_min + obstacle_index * msg->angle_increment;
            auto cmd = geometry_msgs::msg::Twist();
            if (angle_to_obstacle > 0)
                {cmd.linear.x = 0.2;
                    cmd.angular.z = -0.1;}
            else
                {cmd.linear.x = -0.2;
                    cmd.angular.z = 0.1;}
            
            movement_pub_->publish(cmd);
            stuck_counter = 0;
            y = 1;
            rclcpp::sleep_for(std::chrono::seconds(2));
            cmd.linear.x = 1.0;
            dealt = 1;
            movement_pub_->publish(cmd);
            dealing_obstacle = 0;
        }

    void gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {
        if (dealing_obstacle == 0)
        {auto twist_msg = geometry_msgs::msg::Twist();
        current_lat_ = msg->latitude;
        current_lon_ = msg->longitude;
        temp = msg;
        if (flag == 0)
        {if (local_min == 0 )
            {twist_msg.linear.x = 0.05;
            twist_msg.angular.z = -0.1;
            movement_pub_->publish(twist_msg);}
        else
        {twist_msg.linear.x = -0.05;
            twist_msg.angular.z = 0.1;
            movement_pub_->publish(twist_msg);
        }
            y = 0;
            flag = 1;}

std::tie(permd, permb) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);

if (target_lon_ - current_lon_ > 0)
{permb += 180; }


    if (((yaw + permb < 360.5) && (yaw+permb > 359.5)) && flag == 1)
        {flag = 2;
            
            std::cout << "aligned" << std::endl;
            if (flag == 2)
        {twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);}
        flag = 3;
    
    }

    if (flag == 3 && ((yaw + permb < 360.5) && (yaw+permb > 359.5)))
    {

    std::tie(permd, permb) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
        twist_msg.angular.z = 0.0;
    twist_msg.linear.x = 0.5;
    movement_pub_->publish(twist_msg);
    auto inter = std::chrono::milliseconds(static_cast<int>(permd * 1000));
    timer_ = this->create_wall_timer(inter, std::bind(&GpsNavigator::end, this));
    
    
    }

    }
    else
    {}
                
    }

void end(){
    flag = 999;
auto twist_msg = geometry_msgs::msg::Twist();
twist_msg.linear.x = 0.0;
twist_msg.angular.z = 0.0;
movement_pub_->publish(twist_msg);

std::tie(permd, permb) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
if (permd <= 1.5)
{RCLCPP_INFO(this->get_logger(), "Target Reached\n");
    target_reached = 1;
    rclcpp::shutdown();
    exit(0);
}
else
{flag = 0;
    gpsCallback(temp);}
}

void imuCallback(const sensor_msgs::msg::Imu::SharedPtr msg) {
    // Extract quaternion from the IMU message
    double qx = msg->orientation.x;
        double qy = msg->orientation.y;
        double qz = msg->orientation.z;
        double qw = msg->orientation.w;

        double siny_cosp = 2 * (qw * qz + qx * qy);
        double cosy_cosp = 1 - 2 * (qy * qy + qz * qz);
        yaw = atan2(siny_cosp, cosy_cosp) * (180.0 / M_PI);

        if (yaw < 0)
            yaw += 360.0;
 
}

    std::pair<long double, long double> calculateDistanceAndBearing(long double lat1, long double lon1, long double lat2, long double lon2) {
        const long double deg_to_rad = M_PI / 180.0;
        lat1 *= deg_to_rad;
        lon1 *= deg_to_rad;
        lat2 *= deg_to_rad;
        lon2 *= deg_to_rad;

        const long double dlat = lat2 - lat1;
        const long double dlon = lon2 - lon1;

        const long double a = std::sin(dlat / 2) * std::sin(dlat / 2) + std::cos(lat1) * std::cos(lat2) * std::sin(dlon / 2) * std::sin(dlon / 2);
        const long double c = 2 * std::atan2(std::sqrt(a), std::sqrt(1 - a));
        long double distance =  c * 6378137;

        long double y = std::sin(dlon) * std::cos(lat2);
        long double x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dlon);
        long double bearing = std::atan2(y, x);

        bearing *= (180.0 / M_PI);

        if (bearing < 0)
        {bearing += 360;}

        if (bearing > 180)
        {bearing -= 180;}

        return {distance, bearing};
    }




};






int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GpsNavigator>());
    rclcpp::shutdown();
    return 0;
}