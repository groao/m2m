# A module for create by Virtualization TLON Distributed System
# Copycenter (C) 2019
# Giuseppe Roa Osorio <groao@unal.edu.co>
# Grupo de investigación TLÖN
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU Lesser Public License for more details.
#
# You should have received a copy of the GNU Lesser Public License
# along with this program.  If not, see [http://www.gnu.org/licenses/].
#
#__author__ = "Giuseppe Roa Osorio"
#__copyright__ = "Copyright 2019, TLON group"
#__license__ = "LGPL"
#__version__ = "1"
#__email__ = "groao@unal.edu.co"
#__status__ = "Development"

#----------Run_Sim.sh-------------------------------
#!/bin/bash

#--------- VARIABLE DEFINITIONS --------------------

usage="$(basename "$0") [-h] [-x NUM] [-y NUM] [-n NUM] [-p NUM] [-s NUM] [-z NUM] [-e NUM] [-t NUM] [-m NUM] [-f NUM] [-b BOOL] [-i BOOL] [-o NUM]
    Tiny script to automize running of a ns3 scenario
    Options:
    -h	      Show this help text. Default [0]
    -x NUM    Set seed. Default [1]
    -y NUM    Set number of run. Default [1]
    -n NUM    Number of nodes the scenario will have. Default [10]
    -p NUM    Packet Size in bytes. Default [256]
    -s NUM    Number of iterations. Default [5]
    -z NUM    Field size in meters. Default [500.0]
    -e NUM    Number of failure nodes. Default [0]
    -t NUM    Simulation time in seconds. Default [120.0]
    -m NUM    Percentage of nodes with mobility. Default [0.7]
    -f NUM    Number of traffic flows. Default [100]
    -b BOOL   Mobility Model (true->GaussMarkov or false->Randomwaypoint). Default [true]
    -i BOOL   IP Version (true->IPv6 or false->IPv4). Default [true]
    -o BOOL   Traffic Model. Default [1] ->Poisson"

SEED=1
RUN=1
NUMNODES=10
PACKETSIZE=256
NUMITERATIONS=5
FIELDSIZE=500.0
FAILNODES=0
SIMTIME=120.0
PERMOBIL=0.7
FLOWS=100
MOBMODEL='true'
IPVERSION='true'
TRAFFICMO=1

#--------- OPTIONS CHOICE ------------------------

while getopts ':x:y:n:p:s:z:e:t:m:f:b:i:o:h' option
do
 case "${option}"
 in
 x)
        SEED=${OPTARG}
        ;;
 y)
        RUN=${OPTARG}
        ;;
 n)
	NUMNODES=${OPTARG}
	;;
 p)
	PACKETSIZE=${OPTARG}
	;;
 s)
        NUMITERATIONS=${OPTARG}
        ;;
 z)
        FIELDSIZE=${OPTARG}
        ;;
 e)
        FAILNODES=${OPTARG}
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
 o)
        TRAFFICMO=${OPTARG}
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
    echo "----------------- RUN = $i -------------------------"
    num=$((NUMNODES))
    echo "./waf --run 'm2m-example --Seed=$SEED --Run=$RUN --numNodes=$num --animFile=iteration$i.xml --pcapFile=iteration$i --totalTime=$SIMTIME --FieldSize=$FIELDSIZE --NumFailureNodes=$FAILNODES --PerMobil=$PERMOBIL --PacketSize=$PACKETSIZE --flows=$FLOWS --mobModel=$MOBMODEL --IPv=$IPVERSION --traffic=$TRAFFICMO'" | bash
    RUN=$((RUN+$i))
    SEED=$((SEED+$i))
done

#-------------EoF-------------------------------------
exit 0