// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from tutorial_interfaces:srv/Display.idl
// generated code does not contain a copyright notice

#ifndef TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__BUILDER_HPP_
#define TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "tutorial_interfaces/srv/detail/display__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace tutorial_interfaces
{

namespace srv
{

namespace builder
{

class Init_Display_Request_number
{
public:
  Init_Display_Request_number()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  ::tutorial_interfaces::srv::Display_Request number(::tutorial_interfaces::srv::Display_Request::_number_type arg)
  {
    msg_.number = std::move(arg);
    return std::move(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Request msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::tutorial_interfaces::srv::Display_Request>()
{
  return tutorial_interfaces::srv::builder::Init_Display_Request_number();
}

}  // namespace tutorial_interfaces


namespace tutorial_interfaces
{

namespace srv
{

namespace builder
{

class Init_Display_Response_total
{
public:
  explicit Init_Display_Response_total(::tutorial_interfaces::srv::Display_Response & msg)
  : msg_(msg)
  {}
  ::tutorial_interfaces::srv::Display_Response total(::tutorial_interfaces::srv::Display_Response::_total_type arg)
  {
    msg_.total = std::move(arg);
    return std::move(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Response msg_;
};

class Init_Display_Response_unit_price
{
public:
  explicit Init_Display_Response_unit_price(::tutorial_interfaces::srv::Display_Response & msg)
  : msg_(msg)
  {}
  Init_Display_Response_total unit_price(::tutorial_interfaces::srv::Display_Response::_unit_price_type arg)
  {
    msg_.unit_price = std::move(arg);
    return Init_Display_Response_total(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Response msg_;
};

class Init_Display_Response_unit_no
{
public:
  explicit Init_Display_Response_unit_no(::tutorial_interfaces::srv::Display_Response & msg)
  : msg_(msg)
  {}
  Init_Display_Response_unit_price unit_no(::tutorial_interfaces::srv::Display_Response::_unit_no_type arg)
  {
    msg_.unit_no = std::move(arg);
    return Init_Display_Response_unit_price(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Response msg_;
};

class Init_Display_Response_unit_type
{
public:
  explicit Init_Display_Response_unit_type(::tutorial_interfaces::srv::Display_Response & msg)
  : msg_(msg)
  {}
  Init_Display_Response_unit_no unit_type(::tutorial_interfaces::srv::Display_Response::_unit_type_type arg)
  {
    msg_.unit_type = std::move(arg);
    return Init_Display_Response_unit_no(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Response msg_;
};

class Init_Display_Response_time
{
public:
  explicit Init_Display_Response_time(::tutorial_interfaces::srv::Display_Response & msg)
  : msg_(msg)
  {}
  Init_Display_Response_unit_type time(::tutorial_interfaces::srv::Display_Response::_time_type arg)
  {
    msg_.time = std::move(arg);
    return Init_Display_Response_unit_type(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Response msg_;
};

class Init_Display_Response_bill_no
{
public:
  Init_Display_Response_bill_no()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Display_Response_time bill_no(::tutorial_interfaces::srv::Display_Response::_bill_no_type arg)
  {
    msg_.bill_no = std::move(arg);
    return Init_Display_Response_time(msg_);
  }

private:
  ::tutorial_interfaces::srv::Display_Response msg_;
};

}  // namespace builder

}  // namespace srv

template<typename MessageType>
auto build();

template<>
inline
auto build<::tutorial_interfaces::srv::Display_Response>()
{
  return tutorial_interfaces::srv::builder::Init_Display_Response_bill_no();
}

}  // namespace tutorial_interfaces

#endif  // TUTORIAL_INTERFACES__SRV__DETAIL__DISPLAY__BUILDER_HPP_
