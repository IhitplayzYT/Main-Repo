#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <std_msgs/msg/string.hpp>
#include <memory>
#include <functional>
#include <chrono>
#include <tutorial_interfaces/msg/num.hpp>

using namespace std::chrono_literals;
int count = 0;
int units = 0 ;
float price = 0;
char ch;
class Publisher : public rclcpp::Node
{
public:
  Publisher()
  : Node("PUBLISHER")
  {
    publisher_ = this->create_publisher<tutorial_interfaces::msg::Num>("topic", 100);  
    publish_bill();
  }

void publish_bill()
    {count++;
        auto bill_msg = tutorial_interfaces::msg::Num();
        bill_msg.bill_no = count;  
        bill_msg.time = "2024-12-06 12:00:00";  
        bill_msg.unit_type = ch;
        bill_msg.unit_no = units;
        bill_msg.unit_price = price;
        bill_msg.total = units*price;

        RCLCPP_INFO(this->get_logger(), "Your bill is number is : %d\nTime of bill : %s\nYou have purchased %d units of Item %c , each unit costing %f\nYour total amount is : %f",bill_msg.bill_no, bill_msg.time.c_str(), bill_msg.unit_no,bill_msg.unit_type,bill_msg.unit_price, bill_msg.total);

        publisher_ ->publish(bill_msg);
    }


 rclcpp::Publisher<tutorial_interfaces::msg::Num>::SharedPtr publisher_;
    rclcpp::TimerBase::SharedPtr timer;

};




int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);

do
{ch = 'z';
  std::cout << "Input the char a:<unita>:cost=10 , b:<unitb>:cost=20 , c:<unitc>:cost=30 or e<end>" << '\n';
  std::cin >> ch ; 
  std::cout << "Input the number of units" << '\n';
  std::cin >> units;
  if (ch == 'a')
  price = 10;
  else if ( ch == 'b')
  price = 20 ;
  else if ( ch == 'c' )
  price = 30 ; 
  else if ( ch == 'e')
  {std::cout << "Response recorded,Thank tou for your patronage" << '\n';
  exit(0);}
  else
  {std::cout << "Invalid charecter input " << '\n' ;
  exit(0);}

  Publisher();
} while (ch != 'e');

  
  rclcpp::shutdown();
  
  return 0;
}