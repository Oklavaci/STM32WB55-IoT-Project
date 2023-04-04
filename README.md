<picture>
    <img
        alt="A pixel art of a Dophin with text: Flipper Zero Official Repo"
        src="/Serial_Photo/LastImage102.png">
</picture>

# Embedded Image Capturing and Transmisson over BLE

- [Start Here: Docs and Guide](https://github.com/Oklavaci/stm32wb_ble_p2p_server/blob/main/StartHere).
- [STM32WPAN: App Files](https://github.com/Oklavaci/stm32wb_ble_p2p_server/tree/main/STM32_WPAN). BLE functionality, services and apps are here.
- [Serial_Photo: Image Processing Files](https://github.com/Oklavaci/stm32wb_ble_p2p_server/tree/main/Serial_Photo). Images are captured raw (565) on STM32WB55. Fodler includes files related to several image processing steps.

# Contributing

Main goal was to build a project that captures video data and transmits over BLE for various IoT applications. In a way we accomplished this task but very tedious and far away from optimality. Any contribution and sugesstion is appreciated.

## I need help

The best place to search for answers is usually resources and wiki. If you can't find the answer there, check our [Discord Server](https://flipp.dev/discord).

## Understanding & Development

Project is written in C, with some pieces and processing files written in C++, python and arm assembly languages. An intermediate level of embedded C knowledge would be enough to understand and develop. Also see [StartHere](https://github.com/Oklavaci/stm32wb_ble_p2p_server/blob/main/StartHere)

## Requirements
For BLE functionality STM32WBxx boards is required.
OV7670 CMOS Sensor is used as image capturer, register setup done accordingly. If you use different sensor you need to change register values in [OV7670_wb55_lib.c](https://github.com/Oklavaci/stm32wb_ble_p2p_server/blob/main/STM32_WPAN/App/OV7670_wb55_lib.c)

Supported in-circuit debuggers (optional but highly recommended):
- ST-Link
- J-Link

# See
- `Functional Diagram.png`
- `Block Diagram.png`
- `PinConfig.png`
- Also see `ReadMe.md` files inside the directories for further details.
