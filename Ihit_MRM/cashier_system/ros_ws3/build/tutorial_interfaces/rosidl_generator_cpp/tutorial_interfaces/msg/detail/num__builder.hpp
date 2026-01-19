// generated from rosidl_generator_cpp/resource/idl__builder.hpp.em
// with input from tutorial_interfaces:msg/Num.idl
// generated code does not contain a copyright notice

#ifndef TUTORIAL_INTERFACES__MSG__DETAIL__NUM__BUILDER_HPP_
#define TUTORIAL_INTERFACES__MSG__DETAIL__NUM__BUILDER_HPP_

#include <algorithm>
#include <utility>

#include "tutorial_interfaces/msg/detail/num__struct.hpp"
#include "rosidl_runtime_cpp/message_initialization.hpp"


namespace tutorial_interfaces
{

namespace msg
{

namespace builder
{

class Init_Num_total
{
public:
  explicit Init_Num_total(::tutorial_interfaces::msg::Num & msg)
  : msg_(msg)
  {}
  ::tutorial_interfaces::msg::Num total(::tutorial_interfaces::msg::Num::_total_type arg)
  {
    msg_.total = std::move(arg);
    return std::move(msg_);
  }

private:
  ::tutorial_interfaces::msg::Num msg_;
};

class Init_Num_unit_price
{
public:
  explicit Init_Num_unit_price(::tutorial_interfaces::msg::Num & msg)
  : msg_(msg)
  {}
  Init_Num_total unit_price(::tutorial_interfaces::msg::Num::_unit_price_type arg)
  {
    msg_.unit_price = std::move(arg);
    return Init_Num_total(msg_);
  }

private:
  ::tutorial_interfaces::msg::Num msg_;
};

class Init_Num_unit_no
{
public:
  explicit Init_Num_unit_no(::tutorial_interfaces::msg::Num & msg)
  : msg_(msg)
  {}
  Init_Num_unit_price unit_no(::tutorial_interfaces::msg::Num::_unit_no_type arg)
  {
    msg_.unit_no = std::move(arg);
    return Init_Num_unit_price(msg_);
  }

private:
  ::tutorial_interfaces::msg::Num msg_;
};

class Init_Num_unit_type
{
public:
  explicit Init_Num_unit_type(::tutorial_interfaces::msg::Num & msg)
  : msg_(msg)
  {}
  Init_Num_unit_no unit_type(::tutorial_interfaces::msg::Num::_unit_type_type arg)
  {
    msg_.unit_type = std::move(arg);
    return Init_Num_unit_no(msg_);
  }

private:
  ::tutorial_interfaces::msg::Num msg_;
};

class Init_Num_time
{
public:
  explicit Init_Num_time(::tutorial_interfaces::msg::Num & msg)
  : msg_(msg)
  {}
  Init_Num_unit_type time(::tutorial_interfaces::msg::Num::_time_type arg)
  {
    msg_.time = std::move(arg);
    return Init_Num_unit_type(msg_);
  }

private:
  ::tutorial_interfaces::msg::Num msg_;
};

class Init_Num_bill_no
{
public:
  Init_Num_bill_no()
  : msg_(::rosidl_runtime_cpp::MessageInitialization::SKIP)
  {}
  Init_Num_time bill_no(::tutorial_interfaces::msg::Num::_bill_no_type arg)
  {
    msg_.bill_no = std::move(arg);
    return Init_Num_time(msg_);
  }

private:
  ::tutorial_interfaces::msg::Num msg_;
};

}  // namespace builder

}  // namespace msg

template<typename MessageType>
auto build();

template<>
inline
auto build<::tutorial_interfaces::msg::Num>()
{
  return tutorial_interfaces::msg::builder::Init_Num_bill_no();
}

}  // namespace tutorial_interfaces

#endif  // TUTORIAL_INTERFACES__MSG__DETAIL__NUM__BUILDER_HPP_
