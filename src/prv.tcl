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

sta::define_cmd_args "trace_path_to_all_sinks_db" {port}
proc trace_path_to_all_sinks_db { args } {
  sta::parse_key_args "trace_path_to_all_sinks_db" args keys {} flags {}

  check_block_exists
  sta::check_argc_eq1 "trace_path_to_all_sinks_db" $args

  prv::trace_path_to_all_sinks_db [lindex $args 0]
}

sta::define_cmd_args "trace_path_to_all_sinks_sta" {port}
proc trace_path_to_all_sinks_sta { args } {
  sta::parse_key_args "trace_path_to_all_sinks_sta" args keys {} flags {}

  check_block_exists
  sta::check_argc_eq1 "trace_path_to_all_sinks_sta" $args

  prv::trace_path_to_all_sinks_sta [lindex $args 0]
}

