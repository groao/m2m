
#----------Run_Sim.sh-------------------------------
#!/bin/bash

#--------- VARIABLE DEFINITIONS --------------------

usage="$(basename "$0") [-h] [-n NUM] [-p NUM] [-s NUM] [-t NUM] [-m NUM] [-f NUM] [-b BOOL] [-i BOOL]
    Tiny script to automize running of a ns3 scenario
    Options:
    -h	      Show this help text. Default [0]
    -n NUM    Number of nodes the scenario will have. Default [10]
    -p NUM    Packet Size in bytes. Default [1024]
    -s NUM    Number of iterations. Default [5]
    -t NUM    Simulation time in seconds. Default [120.0]
    -m NUM    Percentage of nodes with mobility. Default [0.7]
    -f NUM    Number of traffic flows. Default [6]
    -b BOOL   Mobility Model (true->GaussMarkov or false->Randomwaypoint). Default [true]
    -i BOOL   IP Version (true->IPv6 or false->IPv4). Default [true]"
NUMNODES=10
PACKETSIZE=1024
NUMITERATIONS=5
SIMTIME=120.0
PERMOBIL=0.7
FLOWS=6
MOBMODEL='true'
IPVERSION='true'

#--------- OPTIONS CHOICE ------------------------

while getopts ':n:p:s:t:m:f:b:i:h' option
do
 case "${option}"
 in
 n)
	NUMNODES=${OPTARG}
	;;
 p)
	PACKETSIZE=${OPTARG}
	;;
 s)
        NUMITERATIONS=${OPTARG}
        ;;
 t)
        SIMTIME=${OPTARG}
        ;;
 m)
        PERMOBIL=${OPTARG}
        ;;
 f)
        FLOWS=${OPTARG}
        ;;
 b)
        MOBMODEL=${OPTARG}
        ;;
 i)
        IPVERSION=${OPTARG}
        ;;
 h)
	echo "$usage"
	exit 0
	;;
 :)
	echo "Missing argument for -$OPTARG" >&2
	echo "$usage" >&2
	exit 1
	;;
 \?)
	echo "Invalid option: -$OPTARG" >&2
	echo "$usage" >&2
	exit 1
	;;
 esac
done

#------ LOOP EXECUTION -------------------------------

for (( i=1; i<=$NUMITERATIONS; i++ ))

    do
    num=$((NUMNODES))
   echo "./waf --run 'AdHoc_Sim --numNodes=$num --animFile=iteration$i.xml --pcapFile=iteration$i --totalTime=$SIMTIME --PerMobil=$PERMOBIL --PacketSize=$PACKETSIZE --flows=$FLOWS --mobModel=$MOBMODEL --IPv=$IPVERSION'" | bash

done

#-------------EoF-------------------------------------
exit 0
