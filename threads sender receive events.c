// INTEL CONFIDENTIAL
//
// Copyright (C) 2021-2023 Intel Corporation.
//
// This software and the related documents are Intel copyrighted materials,
// and your use of them is governed by the express license under which they
// were provided to you ("License"). Unless the License provides otherwise,
// you may not use, modify, copy, publish, distribute, disclose or transmit
// this software or the related documents without Intel's prior written permission.
//
// This software and the related documents are provided as is, with no express or
// implied warranties, other than those that are expressly stated in the License.

#include <rtems.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <unistd.h>
#include <rtems/bspIo.h>
#include <bsp.h>

#include <DrvErrors.h>

rtems_id senderTaskId, receiverTaskId, messageQueueId;
rtems_name senderTaskName = rtems_build_name('S', 'E', 'N', 'D'), receiverTaskName = rtems_build_name('R', 'E', 'C', 'V');
rtems_event_set receiverEvent;

rtems_task senderTask(rtems_task_argument ignored) {
    (void)ignored;
    rtems_status_code status;
    while (1) {
        status = rtems_event_send(receiverTaskId, RTEMS_EVENT_0);
        if (status != RTEMS_SUCCESSFUL) {
            printf("Fail!\n");
        }
        rtems_task_wake_after(RTEMS_MILLISECONDS_TO_TICKS(1000));
    }
}

rtems_task receiverTask(rtems_task_argument ignored) {
    (void)ignored;
    rtems_status_code status;
    while (1) {
        status = rtems_event_receive(
            RTEMS_EVENT_0,
            RTEMS_WAIT | RTEMS_EVENT_ANY,
            RTEMS_NO_TIMEOUT,
            &receiverEvent
        );
        if (status == RTEMS_SUCCESSFUL) {
            printf("Message received!\n");
        } else {
            printf("ERROR: %d\n", status);
        }
        rtems_task_wake_after(RTEMS_MILLISECONDS_TO_TICKS(1000));
    }
}

rtems_task Init(rtems_task_argument ignored) {
    (void)ignored;
    printf("ceva\n");
    
    rtems_task_create(senderTaskName, RTEMS_MINIMUM_PRIORITY, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &senderTaskId);
    rtems_task_create(receiverTaskName, RTEMS_MINIMUM_PRIORITY, RTEMS_MINIMUM_STACK_SIZE, RTEMS_DEFAULT_MODES, RTEMS_DEFAULT_ATTRIBUTES, &receiverTaskId);

    rtems_task_start(receiverTaskId, receiverTask, 0);//*/
    rtems_task_start(senderTaskId, senderTask, 0);
    
    DrvPrintReturnStatusCode(MYR_DRV_SUCCESS);
    sleep(500);
    rtems_shutdown_executive(0);
}

#define CONFIGURE_APPLICATION_NEEDS_CLOCK_DRIVER
#define CONFIGURE_APPLICATION_NEEDS_CONSOLE_DRIVER

#define CONFIGURE_UNLIMITED_OBJECTS
#define CONFIGURE_UNIFIED_WORK_AREAS

#define CONFIGURE_RTEMS_INIT_TASKS_TABLE

#define CONFIGURE_INIT
#define CONFIGURE_MAXIMUM_EVENTS 1

#include <rtems/confdefs.h>
