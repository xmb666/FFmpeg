/*
 * copyright (c) 2016 Erkki Seppälä <erkki.seppala.ext@nokia.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#ifndef AVUTIL_CHANNEL_LAYOUT_ISOIEC23001_8_H
#define AVUTIL_CHANNEL_LAYOUT_ISOIEC23001_8_H

/** Speaker positions according to ISO/IEC 23001-8 */
typedef enum AVSpeakerPositionISOIEC23001_8 {
    AV_SPEAKER_POSITION_ISOIEC23001_8_L      = 0,   /// Left front
    AV_SPEAKER_POSITION_ISOIEC23001_8_R      = 1,   /// Right front
    AV_SPEAKER_POSITION_ISOIEC23001_8_C      = 2,   /// Centre front
    AV_SPEAKER_POSITION_ISOIEC23001_8_LFE    = 3,   /// Low frequency enhancement
    AV_SPEAKER_POSITION_ISOIEC23001_8_LS     = 4,   /// Left surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_RS     = 5,   /// Right surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_LC     = 6,   /// Left front centre
    AV_SPEAKER_POSITION_ISOIEC23001_8_RC     = 7,   /// Right front centre
    AV_SPEAKER_POSITION_ISOIEC23001_8_LSR    = 8,   /// Rear surround left
    AV_SPEAKER_POSITION_ISOIEC23001_8_RSR    = 9,   /// Rear surround right
    AV_SPEAKER_POSITION_ISOIEC23001_8_CS     = 10,  /// Rear centre
    AV_SPEAKER_POSITION_ISOIEC23001_8_LSD    = 11,  /// Left surround direct
    AV_SPEAKER_POSITION_ISOIEC23001_8_RSD    = 12,  /// Right surround direct
    AV_SPEAKER_POSITION_ISOIEC23001_8_LSS    = 13,  /// Left side surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_RSS    = 14,  /// Right side surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_LW     = 15,  /// Left wide front
    AV_SPEAKER_POSITION_ISOIEC23001_8_RW     = 16,  /// Right wide front
    AV_SPEAKER_POSITION_ISOIEC23001_8_LV     = 17,  /// Left front vertical height
    AV_SPEAKER_POSITION_ISOIEC23001_8_RV     = 18,  /// Right front vertical height
    AV_SPEAKER_POSITION_ISOIEC23001_8_CV     = 19,  /// Centre front vertical height
    AV_SPEAKER_POSITION_ISOIEC23001_8_LVR    = 20,  /// Left surround vertical height rear
    AV_SPEAKER_POSITION_ISOIEC23001_8_RVR    = 21,  /// Right surround vertical height rear
    AV_SPEAKER_POSITION_ISOIEC23001_8_CVR    = 22,  /// Centre vertical height rear
    AV_SPEAKER_POSITION_ISOIEC23001_8_LVSS   = 23,  /// Left vertical height side surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_RVSS   = 24,  /// Right vertical height side surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_TS     = 25,  /// Top centre surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_LFE2   = 26,  /// E2 Low frequency enhancement 2
    AV_SPEAKER_POSITION_ISOIEC23001_8_LB     = 27,  /// Left front vertical bottom
    AV_SPEAKER_POSITION_ISOIEC23001_8_RB     = 28,  /// Right front vertical bottom
    AV_SPEAKER_POSITION_ISOIEC23001_8_CB     = 29,  /// Centre front vertical bottom
    AV_SPEAKER_POSITION_ISOIEC23001_8_LVS    = 30,  /// Left vertical height surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_RVS    = 31,  /// Right vertical height surround
                                                     /// 32-45 Reserved
    AV_SPEAKER_POSITION_ISOIEC23001_8_LFE3   = 36,  /// E3 Low frequency enhancement 3
    AV_SPEAKER_POSITION_ISOIEC23001_8_LEOS   = 37,  /// Left edge of screen
    AV_SPEAKER_POSITION_ISOIEC23001_8_REOS   = 38,  /// Right edge of screen
    AV_SPEAKER_POSITION_ISOIEC23001_8_HWBCAL = 39,  /// half-way between centre of screen and left edge of screen
    AV_SPEAKER_POSITION_ISOIEC23001_8_HWBCAR = 40,  /// half-way between centre of screen and right edge of screen
    AV_SPEAKER_POSITION_ISOIEC23001_8_LBS    = 41,  /// Left back surround
    AV_SPEAKER_POSITION_ISOIEC23001_8_RBS    = 42,  /// Right back surround
                                                     /// 43–125 Reserved
    AV_SPEAKER_POSITION_ISOIEC23001_8_EXPL   = 126, /// Explicit position (see text)
                                                     /// 127 Unknown / undefined
} AVSpeakerPositionISOIEC23001_8;

/** Channel layouts according to ISO/IEC 23001-8 */
typedef enum AVChannelLayoutISOIEC23001_8 {
    AV_CH_LAYOUT_ISOIEC23001_8_ANY,
    AV_CH_LAYOUT_ISOIEC23001_8_1_0_0,   ///  1 centre front
    AV_CH_LAYOUT_ISOIEC23001_8_2_0_0,   ///  2 left front,   right front
    AV_CH_LAYOUT_ISOIEC23001_8_3_0_0,   ///  3 centre front, left front,  right front
    AV_CH_LAYOUT_ISOIEC23001_8_3_1_0,   ///  4 centre front, left front,  right front,  rear centre
    AV_CH_LAYOUT_ISOIEC23001_8_3_2_0,   ///  5 centre front, left front,  right front,  left surround,  right surround
    AV_CH_LAYOUT_ISOIEC23001_8_3_2_1,   ///  6 centre front, left front,  right front,  left surround,  right surround,  LFE
    AV_CH_LAYOUT_ISOIEC23001_8_5_2_1A,  ///  7 centre front, left front centre,  right front centre,  left front,  right front,  left surround,  right surround,  LFE
    AV_CH_LAYOUT_ISOIEC23001_8_1P1,     ///  8 channel1      channel2
    AV_CH_LAYOUT_ISOIEC23001_8_2_1_0,   ///  9 left front,   right front,  rear centre
    AV_CH_LAYOUT_ISOIEC23001_8_2_2_0,   /// 10 left front,   right front,  left surround,  right surround
    AV_CH_LAYOUT_ISOIEC23001_8_3_3_1,   /// 11 centre front, left front,  right front,  left surround,  right surround,  rear centre,  LFE
    AV_CH_LAYOUT_ISOIEC23001_8_3_4_1,   /// 12 centre front, left front,   right front,   left surround,   right surround,   rear surround left,   rear surround right,   LFE
    AV_CH_LAYOUT_ISOIEC23001_8_11_11_2, /// 13 centre front, left front centre,  right front centre,  left front,  right front,  left side surround,  right side surround,  rear left surround,  rear right surround,  rear centre,  left front LFE,  right front LFE,  centre front vertical height,  left front vertical height,  right front vertical height,  left vertical height side surround,  r       ight vertical height side surround,  top centre surround,  left surround vertical height rear,  r       ight surround vertical height rear ,  centre vertical height rear,  centre front vertical bottom,  left front vertical bottom,  right front vertical bottom
    AV_CH_LAYOUT_ISOIEC23001_8_5_2_1B,  /// 14 centre front, left front,   right front,   left surround,   right surround,   LFE,   left front vertical height,  right front vertical height
    AV_CH_LAYOUT_ISOIEC23001_8_5_5_2,   /// 15 centre front, left front,   right front,   left side surround,   right side surround,   left surround,   right surround,   left front vertical height,   right front vertical height,   centre vertical height rear,   LFE1,   LFE2
    AV_CH_LAYOUT_ISOIEC23001_8_5_4_1,   /// 16 centre front, left front,   right front,   left surround,   right surround,   LFE,   left front vertical height,   right front vertical height,   left vertical height surround,  right vertical height surround
    AV_CH_LAYOUT_ISOIEC23001_8_6_5_1,   /// 17 centre front, left front,   right front,   left surround,   right surround,   LFE,   left front vertical height,   right front vertical height,   centre front vertical height,  left vertical height surround,  right vertical height surround,  top centre surround
    AV_CH_LAYOUT_ISOIEC23001_8_6_7_1,   /// 18 centre front, left front,   right front,   left surround,   right surround,   left back surround,   right back surround   LFE,   left front vertical height,   right front vertical height,   centre front vertical height,  left vertical height surround,  right vertical height surround,  top centre surround
    AV_CH_LAYOUT_ISOIEC23001_8_5_6_1,   /// 19 centre front, left front,  right front,  left side surround,  right side surround,  rear surround left,  rear surround right,  LFE,  left front vertical height,  right front vertical height,  left surround vertical height rear,  right surround vertical height rear
    AV_CH_LAYOUT_ISOIEC23001_8_7_6_1,   /// 20 centre front, left edge of screen,   right edge of screen,   left front,   right front,   left side surround,   right side surround,   rear surround left,   rear surround right,   LFE,   left front vertical height,  right front vertical height,  left vertical height surround,  right vertical height surround
                                         /// 21..63: reserved
} AVChannelLayoutISOIEC23001_8;

#endif /* AVUTIL_CHANNEL_LAYOUT_ISOIEC23001_8_H */
