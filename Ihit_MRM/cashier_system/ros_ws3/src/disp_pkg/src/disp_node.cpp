#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <tutorial_interfaces/srv/display.hpp>
#include <tutorial_interfaces/msg/num.hpp>
#include <memory>
#include <functional>



/*


 void display_func(const std::shared_ptr<tutorial_interfaces::srv::Display::Request> request,std::shared_ptr<tutorial_interfaces::srv::Display::Response> response)
    {
    request -> number;
    std::cout << "The last bill : " << '\n';
    std::cout << response -> bill_no << '\n';
    std::cout << response -> time << '\n';
    std::cout << response -> unit_type << '\n';
    std::cout << response -> unit_no << '\n';
    std::cout << response -> unit_price << '\n';
    std::cout << response -> total << '\n';
 
}



int main(int argc, char ** argv)
{
  rclcpp::init(argc, argv);

std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("DISPLAY");

  rclcpp::Service<tutorial_interfaces::srv::Display>::SharedPtr service =
    node->create_service<tutorial_interfaces::srv::Display>("DISPLAY", &display_func);
  rclcpp::spin(node);
  rclcpp::shutdown();
  return 0;
}




*/



class Display : public rclcpp::Node
{
public:
    Display() : Node("DISPLAY")
    {
        this->declare_parameter<int>("inventory", 100);
        this->declare_parameter<double>("income", 0.0);

        subscription_ = this->create_subscription<tutorial_interfaces::msg::Num>(
            "bill_topic", 10, std::bind(&Display::on_bill_received, this, std::placeholders::_1));
    }


    void on_bill_received(const tutorial_interfaces::msg::Num::SharedPtr msg)
    {
        int inventorya;
        int inventoryb;
        int inventoryc;
        this->get_parameter("INVENTORYA", inventorya);
        this->get_parameter("INVENTORYB", inventoryb);
        this->get_parameter("INVENTORYC", inventoryc);
        double income;
        this->get_parameter("INCOME", income);

        RCLCPP_INFO(this->get_logger(), "Last Bill - Bill Number: %d, Total: %f",msg->bill_no, msg->total);
        RCLCPP_INFO(this->get_logger(), "Current InventoryA: %d\nCurrent InventoryB: %d\nCurrent InventoryC: %d\n, Income: %f",inventorya,inventoryb,inventoryc,income);
    }

    rclcpp::Subscription<tutorial_interfaces::msg::Num>::SharedPtr subscription_;
};

int main(int argc, char **argv)
{
    rclcpp::init(argc, argv);
    rclcpp::spin(std::make_shared<Display>());
    rclcpp::shutdown();
    return 0;
}
