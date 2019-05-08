#
# Copyright 2018 Tempow
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

macro (tcl_register_suite suite)
  add_custom_target(check_tcl_${suite}
    COMMAND ${CMAKE_CTEST_COMMAND} -R '^test_tcl_${suite}.*' --timeout 4)
endmacro ()

macro (tcl_register_test name)
  string(REPLACE "_" ";" parts ${name})
  list(GET parts 0 suite)

  add_executable(test_tcl_${name} ${name}.c test.h)
  target_link_libraries(test_tcl_${name} tcl)
  add_test(test_tcl_${name} test_tcl_${name})
  add_dependencies(check test_tcl_${name})
  add_dependencies(check_tcl_${suite} test_tcl_${name})
  add_dependencies(test_tcl_${name} tcl)
endmacro ()

tcl_register_suite(coro)
tcl_register_test(coro_init)
tcl_register_test(coro_call)
tcl_register_test(coro_call_return_value)
tcl_register_test(coro_call_to_parameter)
tcl_register_test(coro_is_done)
tcl_register_test(coro_resume_caller)
tcl_register_test(coro_yield_return_value)
tcl_register_test(coro_generator)

tcl_register_suite(vector)
tcl_register_test(vector_init)
tcl_register_test(vector_push_pop)

tcl_register_suite(queue)
tcl_register_test(queue_init)
tcl_register_test(queue_basic)

tcl_register_suite(stack)
tcl_register_test(stack_init)
tcl_register_test(stack_basic)

tcl_register_suite(set)
tcl_register_test(set_init)
tcl_register_test(set_insert_simple)
tcl_register_test(set_insert_remove)
if (NOT MSVC OR CMAKE_BUILD_TYPE MATCHES "Release")
  tcl_register_test(set_insert_same)
endif ()

tcl_register_suite(map)
tcl_register_test(map_init)
tcl_register_test(map_insert_simple)
tcl_register_test(map_insert_remove)
if (NOT MSVC OR CMAKE_BUILD_TYPE MATCHES "Release")
  tcl_register_test(map_insert_same)
endif ()

tcl_register_suite(sema)
tcl_register_test(sema_init)
tcl_register_test(sema_init_value)
tcl_register_test(sema_trywait)
tcl_register_test(sema_trywait_after_post)
tcl_register_test(sema_ensure_wait)

tcl_register_suite(mutex)
tcl_register_test(mutex_init)

tcl_register_suite(future)
tcl_register_test(future_init)
tcl_register_test(future_init_imediate)

tcl_register_suite(reactor)
tcl_register_test(reactor_init)
tcl_register_test(reactor_stop_start)
tcl_register_test(reactor_stop_start_repeat)
if (TCL_THREADING)
  tcl_register_test(reactor_start_wait_stop)
  tcl_register_test(reactor_unregister_from_callback)
  tcl_register_test(reactor_unregister_from_worker)
endif ()

tcl_register_suite(blocking)
tcl_register_test(blocking_queue_init)
tcl_register_test(blocking_queue_empty)
tcl_register_test(blocking_queue_length)
tcl_register_test(blocking_queue_push_pop)

tcl_register_suite(worker)
tcl_register_test(worker_init)

tcl_register_suite(alarm)
tcl_register_test(alarm_simple_destroy)
tcl_register_test(alarm_simple_cancel)
tcl_register_test(alarm_cancel)
tcl_register_test(alarm_cancel_idempotent)
tcl_register_test(alarm_set_short)
tcl_register_test(alarm_set_short_periodic)
tcl_register_test(alarm_set_zero_periodic)
tcl_register_test(alarm_set_zero_periodic2)
tcl_register_test(alarm_set_long)
tcl_register_test(alarm_set_short_short)
tcl_register_test(alarm_set_short_long)
tcl_register_test(alarm_set_long_long)
tcl_register_test(alarm_is_scheduled)
tcl_register_test(alarm_callback_ordering)
tcl_register_test(alarm_callback_ordering_on_queue)
tcl_register_test(alarm_unregister_processing_queue)
tcl_register_test(alarm_periodic_unregister_processing_queue)

tcl_register_suite(reader)
tcl_register_test(reader_init)
if (NOT MSVC)
  tcl_register_test(reader_small_data)
  tcl_register_test(reader_multibyte_data)
endif ()
