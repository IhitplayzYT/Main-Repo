/*
 * Copyright (C) 2026 Ihit Rajesh Acharya
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, version 3.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY.
 */
pub mod utils {
    #[allow(non_snake_case, non_camel_case_types)]
    pub fn DBG_STR(inject: &str) -> String {
        format!(
            "****** DEBUG ******\nFile: {}\nLine: {}\nCol: {}\n{}*******************\n",
            file!(),
            line!(),
            column!(),
            inject
        )
    }

const USAGE_STR: &str = "\n
Usage: ZecroVizR [OPTIONS]

Options:
  -c, --vcpus <N>    Number of virtual CPUs (default: 1)
      --smp          Enable SMP mode (irqchip + PIT; requires --vcpus >= 2)
      --dirty-log    Enable KVM dirty-page logging
  -a, --cmdline <S>  Kernel command line string
  -h, --help         Show this message
 
Modes:
  (no flags)          single-thread: 1 vCPU, inline run loop
  --vcpus 4           multi-thread:  4 vCPUs, no irqchip
  --vcpus 4 --smp     SMP:           4 vCPUs, in-kernel irqchip + PIT
";



}
