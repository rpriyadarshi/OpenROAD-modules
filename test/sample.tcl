# Load basic scripts
puts "Sample: Loading basic OpenROAD scripts."
source $::env(SCRIPTS_DIR)/load.tcl

# Load the design
puts "Sample: Loading ODB database."
load_design 2_1_floorplan.odb 1_synth.sdc

# Try getting the port list
puts "Sample: Extracting all ports."
set ports [get_ports *]

# Try printing the port names
puts "Sample: Displaying port names."
foreach {port} $ports {
    #set name [get_property -object_type port $port name]
    set name [get_full_name $port]
    puts $name
}

# Pick one port for bfs run
set start [lindex $ports 6]
set start_name [get_full_name $start]
puts "Sample: Picked up a port $start_name for tracing."

# BFS run
proc bfs {start} {
    set queue $start
    set visited {}

    while {[llength $queue] > 0} {
        # Top
        set curr [lindex $queue 0]
        # Pop
        set queue [lrange $queue 1 end]

        set name [get_full_name $curr]
        puts $name

        if {[lsearch $visited $curr] != -1} {
            continue
        }
        lappend visited $curr
        
        set fanout [get_fanout -from $curr]
        # Push
        set queue [list {*}$queue {*}$fanout]
    }
}

puts "Sample: Running fanout netlist tracing from $start in tcl."
bfs $start

puts "Sample: Running fanout netlist tracing from $start in C++ using odb."
trace_path_to_all_sinks_db [get_full_name $start]

puts "Sample: Running fanout netlist tracing from $start in C++ using sta."
trace_path_to_all_sinks_sta [get_full_name $start]

