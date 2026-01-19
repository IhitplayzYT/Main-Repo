#include <iostream>
#include <rclcpp/rclcpp.hpp>
#include <memory>
#include <functional>
#include <chrono>
#include <tutorial_interfaces/msg/num.hpp>
#include <tutorial_interfaces/srv/display.hpp>

using std::placeholders::_1;

int inventorya = 100;
int inventoryb = 100;
int inventoryc = 100;
double total_income = 0.00;
double income = 0.00;

class Subscriber : public rclcpp::Node
{
public:
  Subscriber()
  : Node("SUBSCRIBER")
  {


    this -> declare_parameter("INCOME",0.00);
    this -> declare_parameter("TOTAL_INCOME",0.00);
    this -> declare_parameter("INVENTORYA",100);
    this -> declare_parameter("INVENTORYB",100);
    this -> declare_parameter("INVENTORYC",100);
    subscriber_ = this->create_subscription<tutorial_interfaces::msg::Num>("topic", 100 ,std::bind(&Subscriber::stock,this,std::placeholders::_1));  
    
    service_ = this->create_service<tutorial_interfaces::srv::Display>("SERVICE",std::bind(&Subscriber::request, this, std::placeholders::_1, std::placeholders::_2));
    
    }
    

  
    
void stock(const tutorial_interfaces::msg::Num::SharedPtr msg)
    {
        
        if ( msg->unit_type == 'a')
        {
        this->set_parameter(rclcpp::Parameter("INVENTORYA", inventorya - msg->unit_no));
        inventorya = this->get_parameter("INVENTORYA").get_parameter_value().get<int>();}

        else if ( msg->unit_type == 'b')
        {
        this->set_parameter(rclcpp::Parameter("INVENTORYB", inventoryb - msg->unit_no));
        inventoryb = this->get_parameter("INVENTORYB").get_parameter_value().get<int>();}

        else if  ( msg->unit_type == 'c')
        {
        this->set_parameter(rclcpp::Parameter("INVENTORYC", inventoryc - msg->unit_no));
        inventoryc = this->get_parameter("INVENTORYC").get_parameter_value().get<int>();}

        else
        exit(0);


       
        income = this->get_parameter("INCOME").get_parameter_value().get<double>();
        this->set_parameter(rclcpp::Parameter("INCOME", income + msg->total));
        income = this->get_parameter("INCOME").get_parameter_value().get<double>();
        
        total_income = this->get_parameter("TOTAL_INCOME").get_parameter_value().get<double>();
        this->set_parameter(rclcpp::Parameter("TOTAL_INCOME", total_income + income));
        total_income = this->get_parameter("TOTAL_INCOME").get_parameter_value().get<double>();
      
      
      
      
      RCLCPP_INFO(this->get_logger(), "Transaction successful:\nBillnumber : %d\nTotal : %f", msg->bill_no, msg->total);
      std::cout << "INVENTORY:-\nA : " << inventorya << "\nB : " << inventoryb << "\nC : " << inventoryc << '\n';
      std::cout << "Net Income : " << total_income << '\n';

        


    }


void request(const std::shared_ptr<tutorial_interfaces::srv::Display::Request> request,std::shared_ptr<tutorial_interfaces::srv::Display::Response> response)
    {
        
        inventoryc = this->get_parameter("INVENTORYC").get_parameter_value().get<int>();
        inventoryb = this->get_parameter("INVENTORYB").get_parameter_value().get<int>();
        inventorya = this->get_parameter("INVENTORYA").get_parameter_value().get<int>();
       
       total_income = this->get_parameter("TOTAL_INCOME").get_parameter_value().get<double>();

        RCLCPP_INFO(this->get_logger(), "Inventoryof A:B:C resp : %d : %d : %d\nIncome: %f", inventorya,inventoryb,inventoryc, income);

      
    }







rclcpp::Subscription<tutorial_interfaces::msg::Num>::SharedPtr subscriber_;
rclcpp::Service<tutorial_interfaces::srv::Display>::SharedPtr service_;


};




int main(int argc, char * argv[])
{
  rclcpp::init(argc, argv);
  do
  {
    rclcpp::spin(std::make_shared<Subscriber>());
 /*std::shared_ptr<rclcpp::Node> node = rclcpp::Node::make_shared("SUBSCRIBER");
  rclcpp::Client<tutorial_interfaces::srv::Display>::SharedPtr client = node->create_client<tutorial_interfaces::srv::Display>("DISPLAY");

auto request = std::make_shared<tutorial_interfaces::srv::Display::Request>();
  request-> number = atoll(argv[1]);

 auto result = client->async_send_request(request);

  RCLCPP_INFO(rclcpp::get_logger("rclcpp"), "Last bill is : \nBill_no : %d\nTimestamp : %s\nUnit_type : %c\nUnit_no : %d\nUnit_price : %f\nTotal : %f", result.get()->bill_no,result.get()->time,result.get()->unit_type,result.get()->unit_no,result.get()->unit_price,result.get()->total);
std::cout << "sent";

   */
  } while (1 == 1) ; 
  
   rclcpp::shutdown();
  return 0;
}
