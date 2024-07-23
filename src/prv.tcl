#################################################################
#
# Copyright (c) 2024, Daedel Inc.
# All rights reserved.
#
#################################################################

## check if a chip/block has been created and exits with error if not
proc check_block_exists { } {
  set db [ord::get_db]
  set chip [$db getChip]

  if { "$chip" eq "NULL" } {
    utl::error PRV 2 "No Chip exists"
  }

  set block [$chip getBlock]

  if { "$block" eq "NULL" } {
    utl::error PRV 3 "No block exists"
  }
}

sta::define_cmd_args "trace_path_to_all_sinks" {file}
proc trace_path_to_all_sinks { args } {
  sta::parse_key_args "trace_path_to_all_sinks" args keys {} flags {}

  check_block_exists
  sta::check_argc_eq1 "trace_path_to_all_sinks" $args

  prv::trace_path_to_all_sinks [lindex $args 0]
}

