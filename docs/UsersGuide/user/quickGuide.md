# Quick Guide
[FPP Guide](https://fprime-community.github.io/fpp/fpp-users-guide.html) (More extensive Guide on FPP)


## Ports
[FPP Guide on Ports](https://fprime-community.github.io/fpp/fpp-users-guide.html#Defining-Ports)

### Port Kind Chart
| Port Kind | Direction | Synchronous / Asynchronous | Guarded | May Return Data | See Diagram | Notes |
|---|---|---|---|---|---|---|
| output | out | | | | `func4()` | The input side of a connection defines all port properties.|
| sync_input | in | synchronous | no | yes | `func2()` | |
| async_input | in | asynchronous | no | no | `func1()` | |
| guarded_input | in | synchronous | yes | yes | `func3()` | Guarded ports by definition must be synchronous |

![Port Instance Kinds](../media/core2.png)

### Port: FPP Definition to C++ Class Implementation

FPP Port Definition
```
port simplePort(
        val1: F32
    )
```
FPP Port Declaration in Component
```
sync input port valIn: simplePort 
```

## Components
[FPP Guide on Components](https://fprime-community.github.io/fpp/fpp-users-guide.html#Defining-Components)

### Component Types Chart
| Component Type | Has Queue | Has Thread | Outputs | Guarded / Synchronous Ports | Asyncrhonous Ports |
|---|---|---|---|---|---|
| Passive Component | No | No | 0 or more | 0 or more | Not Available
| Active Component | Yes | Yes | 0 or more | 0 or more | 1 or more |
| Queued Component | Yes | No | 0 or more | 1 or more | 1 or more | 

## Commands
[FPP Guide on Commands](https://fprime-community.github.io/fpp/fpp-users-guide.html#Defining-Components_Commands)

### Types of Commands (Synchronization Kind)
* **Async**: Arrives on Message Queue. Dispacted on Active Component's Thread. Dispacthed on the thread of a port invocation if in a Queued Component. 
* **Sync**: Command invokes handler defined by the component, runs on the thread of the caller. 
* **Guarded**: Similar to *Sync*hronous commands, but guarded with a mutex.

### Properties of Commands
1. **Opcode**: Numerical value representing command. Relative to component's base ID. 
2. **Mnemonic**: Text value to uniquely Identify command. Attached to component name to ensure uniqueness. 
3. **Arguments**: Primitives and F' Data types supplied to the command. Sent from the ground.
4. **Syncronization Kind**: Async, Sync, or Guarded. 

### Command: FPP Definition to C++ Class Implementation

## Events
[FPP Guide on Events](https://fprime-community.github.io/fpp/fpp-users-guide.html#Defining-Components_Events)

Events represent a log of activities taken by the embedded system. 
Components with commands should have events to log execution and status. 
C-style string is avaiable to pass message about the Event. 

### Properties of Events
1. **ID**
2. **Name**
3. **Severity**
4. **Arguments**
5. **Format String**

### Event Severity
1. **DIAGNOSTIC**: akin to debug messages.  Usually not sent to the ground.
2. **ACTIVITY_LO**: akin to fine info messages these typically come from background tasks
3. **ACTIVITY_HI**: akin to info messages these typically come from foreground tasks
4. **WARNING_LO**: less severe warning events
5. **WARNING_HI**: high-severity warning events, although the system can still function
6. **FATAL**: fatal events indicating that the system **must** reboot
7. **COMMAND**: events tracing the execution of commands

## Telemetry (Telemetry Channels, Telemetry)
[FPP Guide on Telemetry](https://fprime-community.github.io/fpp/fpp-users-guide.html#Defining-Components_Telemetry)

### Telemetry Properties
1. **ID**
2. **Name**
3. **Data_type**: Data type of the channel
4. **Update**: "on_change" to update only when value changes. Or, field omitted to always downlink. 

