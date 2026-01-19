// generated from rosidl_generator_c/resource/idl__functions.c.em
// with input from tutorial_interfaces:srv/Display.idl
// generated code does not contain a copyright notice
#include "tutorial_interfaces/srv/detail/display__functions.h"

#include <assert.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

#include "rcutils/allocator.h"

bool
tutorial_interfaces__srv__Display_Request__init(tutorial_interfaces__srv__Display_Request * msg)
{
  if (!msg) {
    return false;
  }
  // number
  return true;
}

void
tutorial_interfaces__srv__Display_Request__fini(tutorial_interfaces__srv__Display_Request * msg)
{
  if (!msg) {
    return;
  }
  // number
}

bool
tutorial_interfaces__srv__Display_Request__are_equal(const tutorial_interfaces__srv__Display_Request * lhs, const tutorial_interfaces__srv__Display_Request * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // number
  if (lhs->number != rhs->number) {
    return false;
  }
  return true;
}

bool
tutorial_interfaces__srv__Display_Request__copy(
  const tutorial_interfaces__srv__Display_Request * input,
  tutorial_interfaces__srv__Display_Request * output)
{
  if (!input || !output) {
    return false;
  }
  // number
  output->number = input->number;
  return true;
}

tutorial_interfaces__srv__Display_Request *
tutorial_interfaces__srv__Display_Request__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  tutorial_interfaces__srv__Display_Request * msg = (tutorial_interfaces__srv__Display_Request *)allocator.allocate(sizeof(tutorial_interfaces__srv__Display_Request), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(tutorial_interfaces__srv__Display_Request));
  bool success = tutorial_interfaces__srv__Display_Request__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
tutorial_interfaces__srv__Display_Request__destroy(tutorial_interfaces__srv__Display_Request * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    tutorial_interfaces__srv__Display_Request__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
tutorial_interfaces__srv__Display_Request__Sequence__init(tutorial_interfaces__srv__Display_Request__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  tutorial_interfaces__srv__Display_Request * data = NULL;

  if (size) {
    data = (tutorial_interfaces__srv__Display_Request *)allocator.zero_allocate(size, sizeof(tutorial_interfaces__srv__Display_Request), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = tutorial_interfaces__srv__Display_Request__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        tutorial_interfaces__srv__Display_Request__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
tutorial_interfaces__srv__Display_Request__Sequence__fini(tutorial_interfaces__srv__Display_Request__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      tutorial_interfaces__srv__Display_Request__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

tutorial_interfaces__srv__Display_Request__Sequence *
tutorial_interfaces__srv__Display_Request__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  tutorial_interfaces__srv__Display_Request__Sequence * array = (tutorial_interfaces__srv__Display_Request__Sequence *)allocator.allocate(sizeof(tutorial_interfaces__srv__Display_Request__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = tutorial_interfaces__srv__Display_Request__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
tutorial_interfaces__srv__Display_Request__Sequence__destroy(tutorial_interfaces__srv__Display_Request__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    tutorial_interfaces__srv__Display_Request__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
tutorial_interfaces__srv__Display_Request__Sequence__are_equal(const tutorial_interfaces__srv__Display_Request__Sequence * lhs, const tutorial_interfaces__srv__Display_Request__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!tutorial_interfaces__srv__Display_Request__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
tutorial_interfaces__srv__Display_Request__Sequence__copy(
  const tutorial_interfaces__srv__Display_Request__Sequence * input,
  tutorial_interfaces__srv__Display_Request__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(tutorial_interfaces__srv__Display_Request);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    tutorial_interfaces__srv__Display_Request * data =
      (tutorial_interfaces__srv__Display_Request *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!tutorial_interfaces__srv__Display_Request__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          tutorial_interfaces__srv__Display_Request__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!tutorial_interfaces__srv__Display_Request__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}


// Include directives for member types
// Member `time`
#include "rosidl_runtime_c/string_functions.h"

bool
tutorial_interfaces__srv__Display_Response__init(tutorial_interfaces__srv__Display_Response * msg)
{
  if (!msg) {
    return false;
  }
  // bill_no
  // time
  if (!rosidl_runtime_c__String__init(&msg->time)) {
    tutorial_interfaces__srv__Display_Response__fini(msg);
    return false;
  }
  // unit_type
  // unit_no
  // unit_price
  // total
  return true;
}

void
tutorial_interfaces__srv__Display_Response__fini(tutorial_interfaces__srv__Display_Response * msg)
{
  if (!msg) {
    return;
  }
  // bill_no
  // time
  rosidl_runtime_c__String__fini(&msg->time);
  // unit_type
  // unit_no
  // unit_price
  // total
}

bool
tutorial_interfaces__srv__Display_Response__are_equal(const tutorial_interfaces__srv__Display_Response * lhs, const tutorial_interfaces__srv__Display_Response * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  // bill_no
  if (lhs->bill_no != rhs->bill_no) {
    return false;
  }
  // time
  if (!rosidl_runtime_c__String__are_equal(
      &(lhs->time), &(rhs->time)))
  {
    return false;
  }
  // unit_type
  if (lhs->unit_type != rhs->unit_type) {
    return false;
  }
  // unit_no
  if (lhs->unit_no != rhs->unit_no) {
    return false;
  }
  // unit_price
  if (lhs->unit_price != rhs->unit_price) {
    return false;
  }
  // total
  if (lhs->total != rhs->total) {
    return false;
  }
  return true;
}

bool
tutorial_interfaces__srv__Display_Response__copy(
  const tutorial_interfaces__srv__Display_Response * input,
  tutorial_interfaces__srv__Display_Response * output)
{
  if (!input || !output) {
    return false;
  }
  // bill_no
  output->bill_no = input->bill_no;
  // time
  if (!rosidl_runtime_c__String__copy(
      &(input->time), &(output->time)))
  {
    return false;
  }
  // unit_type
  output->unit_type = input->unit_type;
  // unit_no
  output->unit_no = input->unit_no;
  // unit_price
  output->unit_price = input->unit_price;
  // total
  output->total = input->total;
  return true;
}

tutorial_interfaces__srv__Display_Response *
tutorial_interfaces__srv__Display_Response__create()
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  tutorial_interfaces__srv__Display_Response * msg = (tutorial_interfaces__srv__Display_Response *)allocator.allocate(sizeof(tutorial_interfaces__srv__Display_Response), allocator.state);
  if (!msg) {
    return NULL;
  }
  memset(msg, 0, sizeof(tutorial_interfaces__srv__Display_Response));
  bool success = tutorial_interfaces__srv__Display_Response__init(msg);
  if (!success) {
    allocator.deallocate(msg, allocator.state);
    return NULL;
  }
  return msg;
}

void
tutorial_interfaces__srv__Display_Response__destroy(tutorial_interfaces__srv__Display_Response * msg)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (msg) {
    tutorial_interfaces__srv__Display_Response__fini(msg);
  }
  allocator.deallocate(msg, allocator.state);
}


bool
tutorial_interfaces__srv__Display_Response__Sequence__init(tutorial_interfaces__srv__Display_Response__Sequence * array, size_t size)
{
  if (!array) {
    return false;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  tutorial_interfaces__srv__Display_Response * data = NULL;

  if (size) {
    data = (tutorial_interfaces__srv__Display_Response *)allocator.zero_allocate(size, sizeof(tutorial_interfaces__srv__Display_Response), allocator.state);
    if (!data) {
      return false;
    }
    // initialize all array elements
    size_t i;
    for (i = 0; i < size; ++i) {
      bool success = tutorial_interfaces__srv__Display_Response__init(&data[i]);
      if (!success) {
        break;
      }
    }
    if (i < size) {
      // if initialization failed finalize the already initialized array elements
      for (; i > 0; --i) {
        tutorial_interfaces__srv__Display_Response__fini(&data[i - 1]);
      }
      allocator.deallocate(data, allocator.state);
      return false;
    }
  }
  array->data = data;
  array->size = size;
  array->capacity = size;
  return true;
}

void
tutorial_interfaces__srv__Display_Response__Sequence__fini(tutorial_interfaces__srv__Display_Response__Sequence * array)
{
  if (!array) {
    return;
  }
  rcutils_allocator_t allocator = rcutils_get_default_allocator();

  if (array->data) {
    // ensure that data and capacity values are consistent
    assert(array->capacity > 0);
    // finalize all array elements
    for (size_t i = 0; i < array->capacity; ++i) {
      tutorial_interfaces__srv__Display_Response__fini(&array->data[i]);
    }
    allocator.deallocate(array->data, allocator.state);
    array->data = NULL;
    array->size = 0;
    array->capacity = 0;
  } else {
    // ensure that data, size, and capacity values are consistent
    assert(0 == array->size);
    assert(0 == array->capacity);
  }
}

tutorial_interfaces__srv__Display_Response__Sequence *
tutorial_interfaces__srv__Display_Response__Sequence__create(size_t size)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  tutorial_interfaces__srv__Display_Response__Sequence * array = (tutorial_interfaces__srv__Display_Response__Sequence *)allocator.allocate(sizeof(tutorial_interfaces__srv__Display_Response__Sequence), allocator.state);
  if (!array) {
    return NULL;
  }
  bool success = tutorial_interfaces__srv__Display_Response__Sequence__init(array, size);
  if (!success) {
    allocator.deallocate(array, allocator.state);
    return NULL;
  }
  return array;
}

void
tutorial_interfaces__srv__Display_Response__Sequence__destroy(tutorial_interfaces__srv__Display_Response__Sequence * array)
{
  rcutils_allocator_t allocator = rcutils_get_default_allocator();
  if (array) {
    tutorial_interfaces__srv__Display_Response__Sequence__fini(array);
  }
  allocator.deallocate(array, allocator.state);
}

bool
tutorial_interfaces__srv__Display_Response__Sequence__are_equal(const tutorial_interfaces__srv__Display_Response__Sequence * lhs, const tutorial_interfaces__srv__Display_Response__Sequence * rhs)
{
  if (!lhs || !rhs) {
    return false;
  }
  if (lhs->size != rhs->size) {
    return false;
  }
  for (size_t i = 0; i < lhs->size; ++i) {
    if (!tutorial_interfaces__srv__Display_Response__are_equal(&(lhs->data[i]), &(rhs->data[i]))) {
      return false;
    }
  }
  return true;
}

bool
tutorial_interfaces__srv__Display_Response__Sequence__copy(
  const tutorial_interfaces__srv__Display_Response__Sequence * input,
  tutorial_interfaces__srv__Display_Response__Sequence * output)
{
  if (!input || !output) {
    return false;
  }
  if (output->capacity < input->size) {
    const size_t allocation_size =
      input->size * sizeof(tutorial_interfaces__srv__Display_Response);
    rcutils_allocator_t allocator = rcutils_get_default_allocator();
    tutorial_interfaces__srv__Display_Response * data =
      (tutorial_interfaces__srv__Display_Response *)allocator.reallocate(
      output->data, allocation_size, allocator.state);
    if (!data) {
      return false;
    }
    // If reallocation succeeded, memory may or may not have been moved
    // to fulfill the allocation request, invalidating output->data.
    output->data = data;
    for (size_t i = output->capacity; i < input->size; ++i) {
      if (!tutorial_interfaces__srv__Display_Response__init(&output->data[i])) {
        // If initialization of any new item fails, roll back
        // all previously initialized items. Existing items
        // in output are to be left unmodified.
        for (; i-- > output->capacity; ) {
          tutorial_interfaces__srv__Display_Response__fini(&output->data[i]);
        }
        return false;
      }
    }
    output->capacity = input->size;
  }
  output->size = input->size;
  for (size_t i = 0; i < input->size; ++i) {
    if (!tutorial_interfaces__srv__Display_Response__copy(
        &(input->data[i]), &(output->data[i])))
    {
      return false;
    }
  }
  return true;
}
