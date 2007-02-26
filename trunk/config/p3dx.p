;  Robot parameter file

Section General settings
Class Pioneer            ; general type of robot
Subclass p3dx            ; specific type of robot
RobotRadius 250          ; radius in mm
RobotDiagonal 120        ; half-height to diagonal of octagon
RobotWidth 425           ; width in mm
RobotLength 511          ; length in mm of the whole robot
RobotLengthFront 210     ; length in mm to the front of the robot (if this is 0
                         ; (or non existant) this value will be set to half of
                         ; RobotLength)
RobotLengthRear 301      ; length in mm to the rear of the robot (if this is 0
                         ; (or non existant) this value will be set to half of
                         ; RobotLength)
Holonomic true           ; turns in own radius
MaxRVelocity 500         ; absolute maximum degrees / sec
MaxVelocity 2200         ; absolute maximum mm / sec
HasMoveCommand true      ; has built in move command
RequestIOPackets false   ; automatically request IO packets
RequestEncoderPackets false ; automatically request encoder packets
SwitchToBaudRate 38400   ; switch to this baud if non-0 and supported on robot

Section Conversion factors
AngleConvFactor 0.001800 ; radians per angular unit (2PI/4096)  (old value 0.001534)
DistConvFactor 0.580     ; multiplier to mm from robot units (old value 0.485)
VelConvFactor 1          ; multiplier to mm/sec from robot units
RangeConvFactor 1        ; multiplier to mm from sonar units
DiffConvFactor 0.0056    ; ratio of angular velocity to wheel velocity (unused
                         ; in newer firmware that calculates and returns this)
Vel2Divisor 20           ; divisor for VEL2 commands
GyroScaler 1.626         ; Scaling factor for gyro readings

Section Accessories the robot has
TableSensingIR false     ; if robot has upwards facing table sensing IR
NewTableSensingIR false  ; if table sensing IR are sent in IO packet
FrontBumpers false       ; if robot has a front bump ring
NumFrontBumpers 5        ; number of front bumpers on the robot
RearBumpers false        ; if the robot has a rear bump ring
NumRearBumpers 5         ; number of rear bumpers on the robot

Section IR parameters
IRNum 0                  ; number of IRs on the robot
;  IRUnit <IR Number> <IR Type> <Persistance, cycles> <x position, mm> <y
;  position, mm>

Section Sonar parameters
SonarNum 16              ; number of sonar on the robot
;  SonarUnit <sonarNumber> <x position, mm> <y position, mm> <heading of disc,
;  degrees>
SonarUnit 0 69 136 90
SonarUnit 1 114 119 50
SonarUnit 2 148 78 30
SonarUnit 3 166 27 10
SonarUnit 4 166 -27 -10
SonarUnit 5 148 -78 -30
SonarUnit 6 114 -119 -50
SonarUnit 7 69 -136 -90
SonarUnit 8 -157 -136 -90
SonarUnit 9 -203 -119 -130
SonarUnit 10 -237 -78 -150
SonarUnit 11 -255 -27 -170
SonarUnit 12 -255 27 170
SonarUnit 13 -237 78 150
SonarUnit 14 -203 119 130
SonarUnit 15 -157 136 90

Section Laser parameters
LaserPossessed false     ; if there is a laser on the robot
LaserPort COM3           ; port the laser is on
LaserFlipped false       ; if the laser is upside-down or not
LaserPowerControlled true ; if the power to the laser is controlled by serial
LaserX 18                ; x location of laser, mm
LaserY 0                 ; y location of laser, mm
LaserTh 0                ; rotation of laser, deg
LaserIgnore              ; Readings within a degree of the listed degrees
                         ; (separated by a space) will be ignored

Section Movement control parameters
;  if these are 0 the parameters from robot flash will be used, otherwise these
;  values will be used
SettableVelMaxes true    ; if TransVelMax and RotVelMax can be set
TransVelMax 0            ; maximum desired translational velocity for the robot
RotVelMax 0              ; maximum desired rotational velocity for the robot
SettableAccsDecs true    ; if the accel and decel parameters can be set
TransAccel 0             ; translational acceleration
TransDecel 0             ; translational deceleration
RotAccel 0               ; rotational acceleration
RotDecel 0               ; rotational deceleration
