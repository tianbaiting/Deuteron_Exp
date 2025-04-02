#!/bin/bash

#------> added for setting up the GEANT4
source /data4/Bryan/geant4-11.1.2/install/share/Geant4/geant4make/geant4make.sh
#<------


export SMSIMDIR=/data4/tbt23/smsimulator5.5
export PATH=$PATH:$SMSIMDIR/bin
export G4SMLIBDIR=$SMSIMDIR/smg4lib

export G4SMPHYSICSDIR=$SMSIMDIR/smg4lib/physics
export G4SMACTIONDIR=$SMSIMDIR/smg4lib/action
export G4SMCONSTRUCTIONDIR=$SMSIMDIR/smg4lib/construction
export G4SMDATADIR=$SMSIMDIR/smg4lib/data

export G4SMCPPFLAGS="-I$G4SMLIBDIR/include"
export G4SMLDLIBS="-lsmphysics -lsmaction -lsmconstruction -lsmdata"


#    echo $LD_LIBRARY_PATH
#    echo
#    echo $G4SMLIBDIR
#    echo
#    echo $SMSIMDIR


if [[ $LD_LIBRARY_PATH == "" ]]; then
    export LD_LIBRARY_PATH=$G4SMLIBDIR/lib:$SMSIMDIR/lib:$TARTSYS/lib
else
    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$G4SMLIBDIR/lib:$SMSIMDIR/lib:$TARTSYS/lib
fi

if [[ $LIBRARY_PATH == "" ]]; then
    export LIBRARY_PATH=$G4SMLIBDIR/lib
else
    export LIBRARY_PATH=$LIBRARY_PATH:$G4SMLIBDIR/lib
fi

if [[ $ROOT_INCLUDE_PATH == "" ]]; then
    export ROOT_INCLUDE_PATH=$G4SMLIBDIR/include:$TARTSYS/include
else
    export ROOT_INCLUDE_PATH=$ROOT_INCLUDE_PATH:$G4SMLIBDIR/include::$TARTSYS/include
fi

## ----> for kondo
#export TEMPDIRKONDO=/home/kondo/exp/samurai21/anaroot/mysrc
#if [ -e $TEMPDIRKONDO ];then
#    echo set kondo parameters 
#    export G4VRMLFILE_VIEWER=view3dscene
#    export PATH=$PATH:/home/kondo/exp/geant4/viewer/bin:$TARTSYS/../util/bin
#    export TANASYS=$TEMPDIRKONDO
#    export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$TANASYS/lib
#    export LIBRARY_PATH=$LIBRARY_PATH:$TANASYS/lib
#fi
#unset TEMPDIRKONDO
