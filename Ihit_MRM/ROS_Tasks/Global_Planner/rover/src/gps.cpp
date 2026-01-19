#include <rclcpp/rclcpp.hpp>
#include <geometry_msgs/msg/twist.hpp>
#include <sensor_msgs/msg/nav_sat_fix.hpp>
#include <cmath>
#include <iostream>
#include <chrono>
#include <functional>
#include <memory>
#include <unistd.h>

using namespace std::chrono_literals;
long double target_lat_, target_lon_, current_lat_, current_lon_,x,y,distance, bearing;
int n = 0,m = 0;

class GpsNavigator : public rclcpp::Node {
public:
    GpsNavigator() : Node("gps")
    {
        std::cout << "Input target latitude and longitude:" << std::endl;
        std::cin >> current_lat_;
        std::cin >> current_lon_;
        std::tie(distance, bearing) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
        movement_pub_ = this->create_publisher<geometry_msgs::msg::Twist>("/cmd_vel", 10);
        gps_sub_ = this->create_subscription<sensor_msgs::msg::NavSatFix>("/gps", 10, std::bind(&GpsNavigator::gpsCallback, this, std::placeholders::_1));
    }
    rclcpp::Publisher<geometry_msgs::msg::Twist>::SharedPtr movement_pub_;
    rclcpp::Subscription<sensor_msgs::msg::NavSatFix>::SharedPtr gps_sub_;
    rclcpp::TimerBase::SharedPtr timer_;
    rclcpp::TimerBase::SharedPtr inter_timer_;

    void gpsCallback(const sensor_msgs::msg::NavSatFix::SharedPtr msg) {
        
        current_lat_ = msg->latitude;
        current_lon_ = msg->longitude;
        std::tie(distance, bearing) = calculateDistanceAndBearing(current_lat_, current_lon_, target_lat_, target_lon_);
        navigateToTarget();
    }


    
    void navigateToTarget() {
    int surplus = 1.5;
    auto twist_msg = geometry_msgs::msg::Twist(); 
        
        if (bearing == 0)
        {end();}



        if (n == 0){    
        if (bearing > M_PI)
        {bearing = std::abs(M_PI - bearing);
        if (bearing > M_PI/2)
        surplus = 5.0;
        else
        surplus = 2.5;
        long double time = std::abs(bearing/0.5)+surplus;
        auto inter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(time));       
        twist_msg.angular.z =0.5;
        twist_msg.linear.x = 0.0;
        movement_pub_->publish(twist_msg);
        std::cout << "if " << time;
        inter_timer_ = this->create_wall_timer(inter, std::bind(&GpsNavigator::Straight, this));
    
        }
        else if (bearing < M_PI )
        {long double time = std::abs(bearing/0.5)+surplus;
        auto inter = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(time));       
        twist_msg.angular.z = -0.5;
        twist_msg.linear.x = 0.0;
        movement_pub_->publish(twist_msg);
        std::cout << "elseif " << time;
        inter_timer_ = this->create_wall_timer(inter, std::bind(&GpsNavigator::Straight, this));
        }
        else
        exit(0);
        }  
      n++;
    }

void Straight()
{       auto twist_msg = geometry_msgs::msg::Twist();
        twist_msg.linear.x = 0.0;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        long double t = std::abs(distance/0.5)+0.5;
        auto interval = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::duration<double>(t));
        twist_msg.linear.x = 0.5;
        twist_msg.angular.z = 0.0;
        movement_pub_->publish(twist_msg);
        timer_ = this->create_wall_timer(interval, std::bind(&GpsNavigator::end, this));
}

void end()
{auto twist_msg = geometry_msgs::msg::Twist();
twist_msg.linear.x = 0.0;
twist_msg.angular.z = 0.0;
movement_pub_->publish(twist_msg);
RCLCPP_INFO(this->get_logger(), "Target Reached\n");
rclcpp::shutdown();
exit(0);
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

        y = std::sin(dlon) * std::cos(lat2);
        x = std::cos(lat1) * std::sin(lat2) - std::sin(lat1) * std::cos(lat2) * std::cos(dlon);
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
