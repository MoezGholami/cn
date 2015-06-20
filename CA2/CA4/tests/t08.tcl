# This script is created by NSG2 beta1
# <http://wushoupong.googlepages.com/nsg>

#===================================
#     Simulation parameters setup
#===================================
set val(chan)   Channel/WirelessChannel    ;# channel type
set val(prop)   Propagation/TwoRayGround   ;# radio-propagation model
set val(netif)  Phy/WirelessPhy            ;# network interface type
set val(mac)    Mac/802_11                 ;# MAC type
set val(ifq)    Queue/DropTail/PriQueue    ;# interface queue type
set val(ll)     LL                         ;# link layer type
set val(ant)    Antenna/OmniAntenna        ;# antenna model
set val(ifqlen) 50                         ;# max packet in ifq
set val(nn)     20                         ;# number of mobilenodes
set val(rp)     AODV                       ;# routing protocol
set val(x)      4044                      ;# X dimension of topography
set val(y)      100                      ;# Y dimension of topography
set val(stop)   2.0                         ;# time of simulation end

#===================================
#        Initialization        
#===================================
#Create a ns simulator
set ns [new Simulator]

#Setup topography object
set topo       [new Topography]
$topo load_flatgrid $val(x) $val(y)
create-god $val(nn)

#Open the NS trace file
set tracefile [open out.tr w]
$ns trace-all $tracefile

#Open the NAM trace file
set namfile [open out.nam w]
$ns namtrace-all $namfile
$ns namtrace-all-wireless $namfile $val(x) $val(y)
set chan [new $val(chan)];#Create wireless channel

#===================================
#     Mobile node parameter setup
#===================================
$ns node-config -adhocRouting  $val(rp) \
                -llType        $val(ll) \
                -macType       $val(mac) \
                -ifqType       $val(ifq) \
                -ifqLen        $val(ifqlen) \
                -antType       $val(ant) \
                -propType      $val(prop) \
                -phyType       $val(netif) \
                -channel       $chan \
                -topoInstance  $topo \
                -agentTrace    ON \
                -routerTrace   ON \
                -macTrace      ON \
                -movementTrace ON

#===================================
#        Nodes Definition        
#===================================
#Create 20 nodes
set n0 [$ns node]
$n0 set X_ 1561
$n0 set Y_ 1095
$n0 set Z_ 0.0
$ns initial_node_pos $n0 20
set n1 [$ns node]
$n1 set X_ 1362
$n1 set Y_ 816
$n1 set Z_ 0.0
$ns initial_node_pos $n1 20
set n2 [$ns node]
$n2 set X_ 1773
$n2 set Y_ 791
$n2 set Z_ 0.0
$ns initial_node_pos $n2 20
set n3 [$ns node]
$n3 set X_ 1575
$n3 set Y_ 859
$n3 set Z_ 0.0
$ns initial_node_pos $n3 20
set n4 [$ns node]
$n4 set X_ 1941
$n4 set Y_ 1129
$n4 set Z_ 0.0
$ns initial_node_pos $n4 20
set n5 [$ns node]
$n5 set X_ 1214
$n5 set Y_ 1239
$n5 set Z_ 0.0
$ns initial_node_pos $n5 20
set n6 [$ns node]
$n6 set X_ 964
$n6 set Y_ 871
$n6 set Z_ 0.0
$ns initial_node_pos $n6 20
set n7 [$ns node]
$n7 set X_ 1166
$n7 set Y_ 909
$n7 set Z_ 0.0
$ns initial_node_pos $n7 20
set n8 [$ns node]
$n8 set X_ 1250
$n8 set Y_ 1100
$n8 set Z_ 0.0
$ns initial_node_pos $n8 20
set n9 [$ns node]
$n9 set X_ 1798
$n9 set Y_ 1018
$n9 set Z_ 0.0
$ns initial_node_pos $n9 20
set n10 [$ns node]
$n10 set X_ 2193
$n10 set Y_ 802
$n10 set Z_ 0.0
$ns initial_node_pos $n10 20
set n11 [$ns node]
$n11 set X_ 1912
$n11 set Y_ 479
$n11 set Z_ 0.0
$ns initial_node_pos $n11 20
set n12 [$ns node]
$n12 set X_ 1984
$n12 set Y_ 779
$n12 set Z_ 0.0
$ns initial_node_pos $n12 20
set n13 [$ns node]
$n13 set X_ 2003
$n13 set Y_ 552
$n13 set Z_ 0.0
$ns initial_node_pos $n13 20
set n14 [$ns node]
$n14 set X_ 2209
$n14 set Y_ 584
$n14 set Z_ 0.0
$ns initial_node_pos $n14 20
set n15 [$ns node]
$n15 set X_ 2103
$n15 set Y_ 684
$n15 set Z_ 0.0
$ns initial_node_pos $n15 20
set n16 [$ns node]
$n16 set X_ 1562
$n16 set Y_ 709
$n16 set Z_ 0.0
$ns initial_node_pos $n16 20
set n17 [$ns node]
$n17 set X_ 1534
$n17 set Y_ 502
$n17 set Z_ 0.0
$ns initial_node_pos $n17 20
set n18 [$ns node]
$n18 set X_ 1737
$n18 set Y_ 491
$n18 set Z_ 0.0
$ns initial_node_pos $n18 20
set n19 [$ns node]
$n19 set X_ 1732
$n19 set Y_ 600
$n19 set Z_ 0.0
$ns initial_node_pos $n19 20

#===================================
#        Agents Definition        
#===================================
#Setup a UDP connection
set udp0 [new Agent/UDP]
$ns attach-agent $n5 $udp0
set null1 [new Agent/Null]
$ns attach-agent $n5 $null1
$ns connect $udp0 $null1
$udp0 set packetSize_ 1500

#Setup a UDP connection
set udp3 [new Agent/UDP]
$ns attach-agent $n6 $udp3
set null2 [new Agent/Null]
$ns attach-agent $n6 $null2
$ns connect $udp3 $null2
$udp3 set packetSize_ 1500

#Setup a UDP connection
set udp6 [new Agent/UDP]
$ns attach-agent $n4 $udp6
set null5 [new Agent/Null]
$ns attach-agent $n0 $null5
$ns connect $udp6 $null5
$udp6 set packetSize_ 1500

#Setup a UDP connection
set udp7 [new Agent/UDP]
$ns attach-agent $n17 $udp7
set null8 [new Agent/Null]
$ns attach-agent $n1 $null8
$ns connect $udp7 $null8
$udp7 set packetSize_ 1500

#Setup a UDP connection
set udp10 [new Agent/UDP]
$ns attach-agent $n8 $udp10
set null9 [new Agent/Null]
$ns attach-agent $n6 $null9
$ns connect $udp10 $null9
$udp10 set packetSize_ 1500

#Setup a UDP connection
set udp11 [new Agent/UDP]
$ns attach-agent $n10 $udp11
set null12 [new Agent/Null]
$ns attach-agent $n7 $null12
$ns connect $udp11 $null12
$udp11 set packetSize_ 1500

#Setup a UDP connection
set udp14 [new Agent/UDP]
$ns attach-agent $n18 $udp14
set null13 [new Agent/Null]
$ns attach-agent $n18 $null13
$ns connect $udp14 $null13
$udp14 set packetSize_ 1500

#Setup a UDP connection
set udp15 [new Agent/UDP]
$ns attach-agent $n11 $udp15
set null16 [new Agent/Null]
$ns attach-agent $n7 $null16
$ns connect $udp15 $null16
$udp15 set packetSize_ 1500


#===================================
#        Applications Definition        
#===================================
#Setup a CBR Application over UDP connection
set cbr0 [new Application/Traffic/CBR]
$cbr0 attach-agent $udp0
$cbr0 set packetSize_ 1000
$cbr0 set rate_ 1.0Mb
$cbr0 set random_ null
$ns at 0.0 "$cbr0 start"
$ns at 2.0 "$cbr0 stop"

#Setup a CBR Application over UDP connection
set cbr1 [new Application/Traffic/CBR]
$cbr1 attach-agent $udp3
$cbr1 set packetSize_ 1000
$cbr1 set rate_ 1.0Mb
$cbr1 set random_ null
$ns at 0.0 "$cbr1 start"
$ns at 2.0 "$cbr1 stop"

#Setup a CBR Application over UDP connection
set cbr2 [new Application/Traffic/CBR]
$cbr2 attach-agent $udp6
$cbr2 set packetSize_ 1000
$cbr2 set rate_ 1.0Mb
$cbr2 set random_ null
$ns at 0.0 "$cbr2 start"
$ns at 2.0 "$cbr2 stop"

#Setup a CBR Application over UDP connection
set cbr3 [new Application/Traffic/CBR]
$cbr3 attach-agent $udp7
$cbr3 set packetSize_ 1000
$cbr3 set rate_ 1.0Mb
$cbr3 set random_ null
$ns at 0.0 "$cbr3 start"
$ns at 2.0 "$cbr3 stop"

#Setup a CBR Application over UDP connection
set cbr4 [new Application/Traffic/CBR]
$cbr4 attach-agent $udp10
$cbr4 set packetSize_ 1000
$cbr4 set rate_ 1.0Mb
$cbr4 set random_ null
$ns at 0.0 "$cbr4 start"
$ns at 2.0 "$cbr4 stop"

#Setup a CBR Application over UDP connection
set cbr5 [new Application/Traffic/CBR]
$cbr5 attach-agent $udp11
$cbr5 set packetSize_ 1000
$cbr5 set rate_ 1.0Mb
$cbr5 set random_ null
$ns at 0.0 "$cbr5 start"
$ns at 2.0 "$cbr5 stop"

#Setup a CBR Application over UDP connection
set cbr6 [new Application/Traffic/CBR]
$cbr6 attach-agent $udp14
$cbr6 set packetSize_ 1000
$cbr6 set rate_ 1.0Mb
$cbr6 set random_ null
$ns at 0.0 "$cbr6 start"
$ns at 2.0 "$cbr6 stop"

#Setup a CBR Application over UDP connection
set cbr7 [new Application/Traffic/CBR]
$cbr7 attach-agent $udp15
$cbr7 set packetSize_ 1000
$cbr7 set rate_ 1.0Mb
$cbr7 set random_ null
$ns at 0.0 "$cbr7 start"
$ns at 2.0 "$cbr7 stop"


#===================================
#        Termination        
#===================================
#Define a 'finish' procedure
proc finish {} {
    global ns tracefile namfile
    $ns flush-trace
    close $tracefile
    close $namfile
    exec nam out.nam &
    exit 0
}
for {set i 0} {$i < $val(nn) } { incr i } {
    $ns at $val(stop) "\$n$i reset"
}
$ns at $val(stop) "$ns nam-end-wireless $val(stop)"
$ns at $val(stop) "finish"
$ns at $val(stop) "puts \"done\" ; $ns halt"
$ns run
