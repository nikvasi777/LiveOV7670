//
// Created by indrek on 30.04.2016.
//

#ifndef _CAMERAOV7670_QQVGA_H
#define _CAMERAOV7670_QQVGA_H

#include "base/BufferedCameraOV7670.h"


// 160 x 120 @ 5Hz or less
class CameraOV7670_QQVGA : public BufferedCameraOV7670<160, 120> {

public:
  enum FramesPerSecond {
    FPS_5_Hz,
    FPS_3p33_Hz,
    FPS_2p5_Hz,
    FPS_2_Hz
  };


private:
  FramesPerSecond framesPerSecond;

public:
  CameraOV7670_QQVGA(PixelFormat format, FramesPerSecond fps) :
      BufferedCameraOV7670(Resolution::RESOLUTION_QQVGA_160x120, format, getPreScalerForFps(fps)),
      framesPerSecond(fps)
  {};


  inline void readLine() override __attribute__((always_inline));



private:
  static uint8_t getPreScalerForFps(FramesPerSecond fps) {
    switch (fps) {
      default:
      case FPS_5_Hz:
        return 1;
      case FPS_3p33_Hz:
        return 2;
      case FPS_2p5_Hz:
        return 3;
      case FPS_2_Hz:
        return 4;
    }
  }

};




void CameraOV7670_QQVGA::readLine() {
  pixelBuffer.writeBufferPadding = 0;
  uint16_t bufferIndex = 0;


  if (framesPerSecond == FPS_5_Hz) {

    waitForPixelClockLow();
    while (bufferIndex < getPixelBufferLength()) {
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      pixelBuffer.writeBuffer[bufferIndex++] = readPixelByte();
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      asm volatile("nop");
      pixelBuffer.writeBuffer[bufferIndex++] = readPixelByte();
    }

  } else {

    while (bufferIndex < getPixelBufferLength()) {
      waitForPixelClockRisingEdge();
      pixelBuffer.writeBuffer[bufferIndex++] = readPixelByte();
      waitForPixelClockRisingEdge();
      pixelBuffer.writeBuffer[bufferIndex++] = readPixelByte();
    }
  }

}






#endif //_CAMERAOV7670_QQVGA_H
