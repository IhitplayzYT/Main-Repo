#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
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


using namespace std::chrono_literals;
long double woah,target_lat_, target_lon_, current_lat_,current_lon_,set_lat,set_lon,x_mov,y_mov,distance,bearing,permb,permd,origin_lat_ = 0.00000,origin_lon_ = 0.00000;
int n = 0,m = -1;
int flag = 0 ;
double yaw,yaw_rad;
int q = 0;
int temp = 0;
class GpsNavigator : public rclcpp::Node {
    public:
        GpsNavigator() : Node("gps")
        {
            if (m == -1) {
                std::cout << "Input target latitude and longitude:" << std::endl;
                std::cin >> target_lat_;
                std::cin >> target_lon_;
                m++;
                movement_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 100);
                imu_subscriber_ = this->create_subscription<sensor_msgs::msg::Imu>("/imu", 1000, std::bind(&GpsNavigator::imuCallback, this, std::placeholders::_1));
                gps_sub_ = this->create_subscription<sensor_msgs::msg::NavSatFix>("/gps", 1000, std::bind(&GpsNavigator::gpsCallback, this, std::placeholders::_1));
            }
        }
    
        rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr movement_pub_;
        rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_sub_;
        rclcpp::Subscription<sensor_msgs::msg::Imu>::SharedPtr imu_subscriber_;
        rclcpp::TimerBase::SharedPtr timer_;
        rclcpp::TimerBase::SharedPtr timer_func;
    void gpsCallback( sensor_msgs::msg::NavSatFix::SharedPtr msg) {
        auto twist_msg = geometry_msgs::msg::Twist();
        current_lat_ = msg->latitude;
        current_lon_ = msg->longitude;
        
        if (flag == 0)
        {twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 3.0;
        movement_pub_->publish(twist_msg);
        std::tie(permd, permb) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
        if (permb*(180/M_PI) > 180)
        {permb -= 180;
        temp = 1;}
        flag = 1;
       
}

permb = permb * (180/M_PI);
        
    if (((yaw + permb) > 357 && (yaw + permb) < 363) && temp == 0)
        {std::cout << "oriented" << std::endl;
 
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 3.0;
        movement_pub_->publish(twist_msg);
        flag = 2;
        m++;
        navigateToTarget();
    }
    else if (((yaw + permb) > 178  && (yaw + permb) < 185) && temp == 1)
    {std::cout << "oriented" << std::endl;

        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        flag = 2;
        m++;
        navigateToTarget();
    }
    else
    {}
        current_lat_ = msg->latitude;
        current_lon_ = msg->longitude;
        
    }


   

    void navigateToTarget() {
    auto twist_msg = geometry_msgs::msg::Twist();
    if (n==0 && flag == 2){
        std::cout << " Move" << std::endl;
        std::tie(permd, permb) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
        flag = 1;
        twist_msg.linear.x = 1.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        sleep(permd);
        
    n++;
    
    
    end();
    
    
    }


    }

void end(){
auto twist_msg = geometry_msgs::msg::Twist();
twist_msg.linear.x = 0.0;
twist_msg.angular.z = 0.0;
movement_pub_->publish(twist_msg);
rclcpp::shutdown();
exit(0);
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
            {yaw += 360.0;}
 
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

        return {distance, bearing};
    }




};






int main(int argc, char *argv[]) {
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<GpsNavigator>());
    rclcpp::shutdown();
    return 0;
}
