/*
 ** THIS SOFTWARE IS SUBJECT TO COPYRIGHT PROTECTION AND IS OFFERED ONLY
 ** PURSUANT TO THE 3DFX GLIDE GENERAL PUBLIC LICENSE. THERE IS NO RIGHT
 ** TO USE THE GLIDE TRADEMARK WITHOUT PRIOR WRITTEN PERMISSION OF 3DFX
 ** INTERACTIVE, INC. A COPY OF THIS LICENSE MAY BE OBTAINED FROM THE 
 ** DISTRIBUTOR OR BY CONTACTING 3DFX INTERACTIVE INC(info@3dfx.com). 
 ** THIS PROGRAM IS PROVIDED "AS IS" WITHOUT WARRANTY OF ANY KIND, EITHER 
 ** EXPRESSED OR IMPLIED. SEE THE 3DFX GLIDE GENERAL PUBLIC LICENSE FOR A
 ** FULL TEXT OF THE NON-WARRANTY PROVISIONS.  
 ** 
 ** USE, DUPLICATION OR DISCLOSURE BY THE GOVERNMENT IS SUBJECT TO
 ** RESTRICTIONS AS SET FORTH IN SUBDIVISION (C)(1)(II) OF THE RIGHTS IN
 ** TECHNICAL DATA AND COMPUTER SOFTWARE CLAUSE AT DFARS 252.227-7013,
 ** AND/OR IN SIMILAR OR SUCCESSOR CLAUSES IN THE FAR, DOD OR NASA FAR
 ** SUPPLEMENT. UNPUBLISHED RIGHTS RESERVED UNDER THE COPYRIGHT LAWS OF
 ** THE UNITED STATES.  
 ** 
 ** COPYRIGHT 3DFX INTERACTIVE, INC. 1999, ALL RIGHTS RESERVED
 */

#include <stdlib.h>
#include <stdio.h>
#include <3dfx.h>
#define FX_DLL_DEFINITION
#include <fxdll.h>
#include "fxpci.h"
#include "pcilib.h"

const char *
pciGetClassName( FxU32 class_code, FxU32 deviceID)
{
  switch( class_code ) {
    /* Pre-Class Code Devices        */
  case 0x000000:  return "old style non-VGA device";
  case 0x000100:  return "old style VGA-compatible device";
    /* Mass Storage Controllers */
  case 0x010000:  return "SCSI bus controller";
  case 0x010100:  return "IDE Controller";
  case 0x010200:  return "floppy Controller";
  case 0x010300:  return "IPI bus controller";
  case 0x010400:  return "RAID controller";
  case 0x018000:  return "mass storage controller";
    /* Network Controllers */
  case 0x020000:  return "ethernet controller";
  case 0x020100:  return "token ring controller";
  case 0x020200:  return "FDDI controller";
  case 0x020300:  return "ATM controller";
  case 0x028000:  return "network controller";
    /* Display Adapters */
  case 0x030000:  return "VGA-compatible display controller";
  case 0x030001:  return "8514-compatible display controller";
  case 0x030101:  return "XGA-compatible display controller";
  case 0x038000:  return "display controller";
    /* Multimedia Adapters */
  case 0x040000:  return "video multimedia adapter";
  case 0x040100:  return "audio multimedia adapter";
  case 0x048000:  return "multimedia adapter";
    /* Memory Controllers */
  case 0x050000:  return "RAM controller";
  case 0x050100:  return "Flash controller";
  case 0x058000:  return "memory controller";
    /* Bridges */
  case 0x060000:  
    switch(deviceID) {
    case 0x84c4:    return "450KX/GX (Orion)";
    case 0x1237:    return "440FX (Natoma)";
    case 0x1250:    return "430HX (Triton II)";
    case 0x7030:    return "430VX (Triton II)";
    case 0x1235:    return "430MX (Triton Mobile)";
    case 0x122d:    return "430FX (Triton)";
    case 0x04a3:    return "430NX/LX (Neptune/Mercury)";
    default:        return "Host bridge";
    }
  case 0x060100:  return "ISA bridge";
  case 0x060200:  return "EISA bridge";
  case 0x060300:  return "MCA bridge";
  case 0x060400:  return "PCI-toPCI bridge";
  case 0x060500:  return "PCMCIA bridge";
  case 0x060600:  return "NuBus bridge";
  case 0x060700:  return "CardBus bridge";
  case 0x068000:  return "bridge";
    /* Communications Devices */
  case 0x070000:  return "XT-compatible serial controller";
  case 0x070001:  return "16450-compatible serial controller";
  case 0x070002:  return "16550-compatible serial controller";
  case 0x070100:  return "parallel port";
  case 0x070101:  return "bidirectional parallel port";
  case 0x070102:  return "ECP 1.X compliant parallel port";
  case 0x078000:  return "communications device";
    /* Generic System Peripherals */
  case 0x080000:  return "8259 PIC";
  case 0x080001:  return "ISA PIC";
  case 0x080002:  return "EISA PIC";
  case 0x080100:  return "8237 DMA controller";
  case 0x080101:  return "ISA DMA controller";
  case 0x080102:  return "EISA DMA controller";
  case 0x080200:  return "8254 system timer";
  case 0x080201:  return "ISA system timer";
  case 0x080202:  return "EISA system timer";
  case 0x080300:  return "generic RTC controller";
  case 0x080301:  return "ISA RTC controller";
  case 0x088000:  return "system peripheral";
    /* Input Devices */
  case 0x090000:  return "keyboard controller";
  case 0x090100:  return "pen digitizer";
  case 0x090200:  return "mouse controller";
  case 0x098000:  return "input device";
    /* Docking Stations */
  case 0x0A0000:  return "generic docking station";
  case 0x0A8000:  return "docking station";
    /* Processors */
  case 0x0B0000:  return "386";
  case 0x0B0100:  return "486";
  case 0x0B0200:  return "Pentium";
  case 0x0B1000:  return "Alpha";
  case 0x0B2000:  return "PowerPC";
  case 0x0B4000:  return "co-processor";
    /* Serial Bus Controllers */
  case 0x0C0000:  return "FireWire( IEEE 1394 )";
  case 0x0C0100:  return "ACCESS.bus";
  case 0x0C0200:  return "SSA";
  case 0x0C0300:  return "USB";
  case 0x0C0400:  return "FibreChannel";
  default:                return "???";
  }
} /* end pciGetClassName() */
  


const char *
pciGetVendorName( FxU16 vendor_id )
{
  /* These come vrom http://www.pcisig.com/siginfo */
  switch( vendor_id ) {
  case 0x1002:    return "ATI TECHNOLOGIES INC";
  case 0x1003:    return "ULSI SYSTEMS";
  case 0x1004:    return "VLSI TECHNOLOGY INC";
  case 0x1006:    return "REPLY GROUP";
  case 0x1007:    return "NETFRAME SYSTEMS INC";
  case 0x100a:    return "PHOENIX TECHNOLOGIES";
  case 0x100b:    return "NATIONAL SEMICONDUCTOR CORPORATION";
  case 0x100c:    return "TSENG LABS INC";
  case 0x100d:    return "AST COMPUTER";
  case 0x100e:    return "WEITEK";
  case 0x1010:    return "VIDEO LOGIC LTD";
  case 0x1011:    return "DIGITAL EQUIPMENT CORPORATION";
  case 0x1012:    return "MICRONICS COMPUTERS INC";
  case 0x1013:    return "CIRRUS LOGIC";
  case 0x1014:    return "IBM";
  case 0x1015:    return "LSI LOGIC CORPORATION";
  case 0x1016:    return "ICL PERSONAL SYSTEMS";
  case 0x1017:    return "SPEA SOFTWARE AG";
  case 0x1018:    return "UNISYS CORPORATION";
  case 0x101a:    return "AT&T GIS (NCR)";
  case 0x101b:    return "VITESSE SEMICONDUCTOR";
  case 0x101e:    return "AMERICAN MEGATRENDS";
  case 0x101f:    return "PICTURETEL";
  case 0x1020:    return "HITACHI COMPUTER PRODUCTS";
  case 0x1021:    return "OKI ELECTRIC INDUSTRY CO. LTD.";
  case 0x1022:    return "ADVANCED MICRO DEVICES";
  case 0x1023:    return "TRIDENT MICROSYSTEMS";
  case 0x1025:    return "ACER INCORPORATED";
  case 0x1028:    return "DELL COMPUTER CORPORATION";
  case 0x102b:    return "MATROX GRAPHICS, INC.";
  case 0x102c:    return "CHIPS AND TECHNOLOGIES";
  case 0x102d:    return "WYSE TECHNOLOGY";
  case 0x102e:    return "OLIVETTI ADVANCED TECHNOLOGY";
  case 0x102f:    return "Toshiba";
  case 0x1030:    return "TMC RESEARCH";
  case 0x1031:    return "MIRO COMPUTER PRODUCTS AG";
  case 0x1033:    return "NEC Corporation";
  case 0x1034:    return "FRAMATOME CONNECTORS USA INC.";
  case 0x1035:    return "INDUSTRIAL TECHNOLOGY RESEARCH INSTITUTE";
  case 0x1036:    return "FUTURE DOMAIN CORP.";
  case 0x1037:    return "HITACHI MICRO SYSTEMS";
  case 0x1038:    return "AMP, INC";
  case 0x1039:    return "SILICON INTEGRATED SYSTEMS";
  case 0x103a:    return "SEIKO EPSON CORPORATION";
  case 0x103b:    return "TATUNG CO. OF AMERICA";
  case 0x103c:    return "HEWLETT PACKARD";
  case 0x103e:    return "SOLLIDAY ENGINEERING";
  case 0x103f:    return "SYNOPSYS/LOGIC MODELING GROUP";
  case 0x1040:    return "ACCELGRAPHICS INC.";
  case 0x1042:    return "PC TECHNOLOGY INC";
  case 0x1043:    return "ASUSTEK COMPUTER, INC.";
  case 0x1044:    return "DISTRIBUTED PROCESSING TECHNOLOGY";
  case 0x1045:    return "OPTI INC.";
  case 0x1046:    return "IPC CORPORATION, LTD.";
  case 0x1047:    return "GENOA SYSTEMS CORP";
  case 0x1048:    return "ELSA GMBH";
  case 0x1049:    return "FOUNTAIN TECHNOLOGIES, INC.";
  case 0x104a:    return "SGS THOMSON MICROELECTRONICS";
  case 0x104b:    return "BUSLOGIC";
  case 0x104c:    return "TEXAS INSTRUMENTS";
  case 0x104d:    return "SONY CORPORATION";
  case 0x104e:    return "OAK TECHNOLOGY, INC";
  case 0x104f:    return "CO-TIME COMPUTER LTD";
  case 0x1050:    return "WINBOND ELECTRONICS CORP";
  case 0x1051:    return "ANIGMA, INC.";
  case 0x1053:    return "YOUNG MICRO SYSTEMS";
  case 0x1054:    return "HITACHI, LTD";
  case 0x1057:    return "MOTOROLA";
  case 0x1058:    return "ETRI";
  case 0x1059:    return "TEKNOR MICROSYSTEMS";
  case 0x105a:    return "PROMISE TECHNOLOGY, INC.";
  case 0x105b:    return "FOXCONN INTERNATIONAL, INC.";
  case 0x105c:    return "WIPRO INFOTECH LIMITED";
  case 0x105d:    return "NUMBER 9 VISUAL TECHNOLOGY";
  case 0x105e:    return "VTECH COMPUTERS LTD";
  case 0x105f:    return "INFOTRONIC AMERICA INC";
  case 0x1060:    return "UNITED MICROELECTRONICS";
  case 0x1061:    return "I.I.T.";
  case 0x1062:    return "MASPAR COMPUTER CORP";
  case 0x1063:    return "OCEAN OFFICE AUTOMATION";
  case 0x1064:    return "ALCATEL CIT";
  case 0x1065:    return "TEXAS MICROSYSTEMS";
  case 0x1066:    return "PICOPOWER TECHNOLOGY";
  case 0x1067:    return "MITSUBISHI ELECTRONICS";
  case 0x1068:    return "DIVERSIFIED TECHNOLOGY";
  case 0x1069:    return "MYLEX CORPORATION";
  case 0x106a:    return "ATEN RESEARCH INC";
  case 0x106b:    return "APPLE COMPUTER INC.";
  case 0x106c:    return "HYUNDAI ELECTRONICS AMERICA";
  case 0x106d:    return "SEQUENT COMPUTER SYSTEMS";
  case 0x106e:    return "DFI, INC";
  case 0x106f:    return "CITY GATE DEVELOPMENT LTD";
  case 0x1070:    return "DAEWOO TELECOM LTD";
  case 0x1071:    return "MITAC";
  case 0x1072:    return "GIT CO LTD";
  case 0x1073:    return "YAMAHA CORPORATION";
  case 0x1074:    return "NEXGEN MICROSYSTEMS";
  case 0x1075:    return "ADVANCED INTEGRATION RES.";
  case 0x1077:    return "Q LOGIC";
  case 0x1078:    return "CYRIX CORPORATION";
  case 0x1079:    return "I-BUS";
  case 0x107a:    return "NETWORTH";
  case 0x107b:    return "GATEWAY 2000";
  case 0x107c:    return "LG ELECTRONICS";
  case 0x107d:    return "LEADTEK RESEARCH INC.";
  case 0x107e:    return "INTERPHASE CORPORATION";
  case 0x107f:    return "DATA TECHNOLOGY CORPORATION";
  case 0x1080:    return "CYPRESS SEMICONDUCTOR";
  case 0x1081:    return "RADIUS, INC.";
  case 0x1083:    return "FOREX COMPUTER CORPORATION";
  case 0x1084:    return "PARADOR";
  case 0x1085:    return "TULIP COMPUTERS INT.B.V.";
  case 0x1086:    return "J. BOND COMPUTER SYSTEMS";
  case 0x1088:    return "MICROCOMPUTER SYSTEMS (M) SON";
  case 0x108a:    return "BIT 3 COMPUTER";
  case 0x108c:    return "OAKLEIGH SYSTEMS INC.";
  case 0x108d:    return "OLICOM";
  case 0x108e:    return "SUN MICROSYSTEMS COMPUTER CORP.";
  case 0x108f:    return "SYSTEMSOFT";
  case 0x1090:    return "ENCORE COMPUTER CORPORATION";
  case 0x1091:    return "INTERGRAPH CORPORATION";
  case 0x1092:    return "DIAMOND MULTIMEDIA SYSTEMS";
  case 0x1093:    return "NATIONAL INSTRUMENTS";
  case 0x1094:    return "FIRST INT'L COMPUTERS";
  case 0x1095:    return "CMD TECHNOLOGY INC";
  case 0x1096:    return "ALACRON";
  case 0x1097:    return "APPIAN/ETMA";
  case 0x1098:    return "QUANTUM DESIGNS (H.K.) LTD";
  case 0x1099:    return "SAMSUNG ELECTRONICS CO., LTD";
  case 0x109a:    return "PACKARD BELL";
  case 0x109b:    return "GEMLIGHT COMPUTER LTD.";
  case 0x109c:    return "MEGACHIPS CORPORATION";
  case 0x109d:    return "ZIDA TECHNOLOGIES LTD.";
  case 0x109e:    return "BROOKTREE CORPORATION";
  case 0x109f:    return "TRIGEM COMPUTER INC.";
  case 0x10a0:    return "MEIDENSHA CORPORATION";
  case 0x10a1:    return "JUKO ELECTRONICS IND. CO. LTD";
  case 0x10a2:    return "QUANTUM CORPORATION";
  case 0x10a4:    return "GLOBE MANUFACTURING SALES";
  case 0x10a6:    return "RACAL INTERLAN";
  case 0x10a7:    return "INFORMTECH INDUSTRIAL LTD.";
  case 0x10a8:    return "SIERRA SEMICONDUCTOR";
  case 0x10a9:    return "SILICON GRAPHICS";
  case 0x10aa:    return "ACC MICROELECTRONICS";
  case 0x10ab:    return "DIGICOM";
  case 0x10ac:    return "HONEYWELL, INC.";
  case 0x10ad:    return "SYMPHONY LABS";
  case 0x10ae:    return "CORNERSTONE IMAGING";
  case 0x10af:    return "MICRO COMPUTER SYSYTEMS INC";
  case 0x10b0:    return "CARDEXPERT TECHNOLOGY";
  case 0x10b1:    return "CABLETRON SYSTEMS INC";
  case 0x10b2:    return "RAYTHEON COMPANY";
  case 0x10b3:    return "DATABOOK INC";
  case 0x10b4:    return "STB SYSTEMS INC";
  case 0x10b5:    return "PLX TECHNOLOGY, INC.";
  case 0x10b6:    return "MADGE NETWORKS";
  case 0x10b7:    return "3COM CORPORATION";
  case 0x10b8:    return "STANDARD MICROSYSTEMS";
  case 0x10ba:    return "MITSUBISHI ELECTRIC CORP.";
  case 0x10bb:    return "DAPHA ELECTRONICS CORPORATION";
  case 0x10bc:    return "ADVANCED LOGIC RESEARCH";
  case 0x10bd:    return "SURECOM TECHNOLOGY";
  case 0x10be:    return "TSENGLABS INTERNATIONAL CO.";
  case 0x10bf:    return "MOST INC";
  case 0x10c0:    return "BOCA RESEARCH INC.";
  case 0x10c1:    return "ICM CO., LTD.";
  case 0x10c2:    return "AUSPEX SYSTEMS INC.";
  case 0x10c3:    return "SAMSUNG SEMICONDUCTOR , INC.";
  case 0x10c4:    return "AWARD SOFTWARE";
  case 0x10c5:    return "XEROX CORPORATION";
  case 0x10c6:    return "RAMBUS INC.";
  case 0x10c8:    return "NEOMAGIC CORPORATION";
  case 0x10c9:    return "DATAEXPERT CORPORATION";
  case 0x10ca:    return "FUJITSU MICROELECTR., INC.";
  case 0x10cb:    return "OMRON CORPORATION";
  case 0x10cc:    return "MENTOR ARC INC";
  case 0x10cd:    return "ADVANCED SYSTEM PRODUCTS, INC";
  case 0x10cf:    return "CITICORP TTI";
  case 0x10d0:    return "FUJITSU LIMITED";
  case 0x10d1:    return "FUTUREPLUS SYSTEMS CORP.";
  case 0x10d2:    return "MOLEX INCORPORATED";
  case 0x10d3:    return "JABIL CIRCUIT INC";
  case 0x10d4:    return "HUALON MICROELECTRONICS";
  case 0x10d5:    return "AUTOLOGIC INC.";
  case 0x10d6:    return "CETIA";
  case 0x10d7:    return "BCM ADVANCED RESEARCH";
  case 0x10d8:    return "ADVANCED PERIPHERALS LABS";
  case 0x10d9:    return "MACRONIX, INC.";
  case 0x10da:    return "COMPAQ IPG-AUSTIN";
  case 0x10db:    return "ROHM LSI SYSTEMS, INC.";
  case 0x10dc:    return "CERN/ECP/EDU";
  case 0x10dd:    return "EVANS & SUTHERLAND";
  case 0x10de:    return "NVIDIA CORPORATION";
  case 0x10df:    return "EMULEX CORPORATION";
  case 0x10e0:    return "INTEGRATED MICRO SOLUTIONS INC.";
  case 0x10e1:    return "TEKRAM TECHNOLOGY CO.,LTD.";
  case 0x10e2:    return "APTIX CORPORATION";
  case 0x10e3:    return "NEWBRIDGE MICROSYSTEMS";
  case 0x10e4:    return "TANDEM COMPUTERS";
  case 0x10e5:    return "MICRO INDUSTRIES CORPORATION";
  case 0x10e6:    return "GAINBERY COMPUTER PRODUCTS INC.";
  case 0x10e7:    return "VADEM";
  case 0x10e8:    return "APPLIED MICRO CIRCUITS CORPORATION";
  case 0x10e9:    return "ALPS ELECTRIC CO. LTD.";
  case 0x10ea:    return "INTEGRAPHICS SYSTEMS";
  case 0x10eb:    return "ARTISTS GRAPHICS";
  case 0x10ec:    return "REALTEK SEMICONDUCTOR CO., LTD.";
  case 0x10ed:    return "ASCII CORPORATION";
  case 0x10ee:    return "XILINX, INC.";
  case 0x10ef:    return "RACORE COMPUTER PRODUCTS, INC.";
  case 0x10f0:    return "PERITEK CORPORATION";
  case 0x10f1:    return "TYAN COMPUTER";
  case 0x10f2:    return "ACHME COMPUTER INC.";
  case 0x10f3:    return "ALARIS, INC.";
  case 0x10f4:    return "S-MOS SYSTEMS";
  case 0x10f5:    return "NKK CORPORATION";
  case 0x10f6:    return "CREATIVE ELECTRONIC SYSTEMS SA";
  case 0x10f7:    return "MATSUSHITA ELECTRIC INDUSTRIAL CO., LTD.";
  case 0x10f8:    return "ALTOS INDIA LTD";
  case 0x10f9:    return "PC DIRECT";
  case 0x10fa:    return "TRUEVISION";
  case 0x10fb:    return "THESYS GES. F. MIKROELEKTRONIK MGH";
  case 0x10fc:    return "I-O DATA DEVICE, INC.";
  case 0x10fd:    return "SOYO COMPUTER, INC";
  case 0x10fe:    return "FAST MULTIMEDIA AG";
  case 0x10ff:    return "NCUBE";
  case 0x1100:    return "JAZZ MULTIMEDIA";
  case 0x1101:    return "INITIO CORPORATION";
  case 0x1102:    return "CREATIVE LABS";
  case 0x1103:    return "TRIONES TECHNOLOGIES, INC.";
  case 0x1104:    return "RASTEROPS CORP.";
  case 0x1105:    return "SIGMA DESIGNS, INC";
  case 0x1106:    return "VIA TECHNOLOGIES, INC.";
  case 0x1107:    return "STRATUS COMPUTER";
  case 0x1108:    return "PROTEON, INC.";
  case 0x1109:    return "COGENT DATA TECHNOLOGIES, INC.";
  case 0x110a:    return "SIEMENS NIXDORF AG";
  case 0x110b:    return "CHROMATIC RESEARCH INC.";
  case 0x110c:    return "MINI-MAX TECHNOLOGY, INC.";
  case 0x110d:    return "ZNYX ADVANCED SYSTEMS";
  case 0x110e:    return "CPU TECHNOLOGY";
  case 0x110f:    return "ROSS TECHNOLOGY";
  case 0x1111:    return "SANTA CRUZ OPERATION";
  case 0x1112:    return "RNS";
  case 0x1113:    return "ACCTON TECHNOLOGY CORPORATION";
  case 0x1114:    return "ATMEL CORPORATION";
  case 0x1116:    return "DATA TRANSLATION";
  case 0x1117:    return "DATACUBE, INC";
  case 0x1118:    return "BERG ELECTRONICS";
  case 0x1119:    return "VORTEX COMPUTERSYSTEME GMBH";
  case 0x111a:    return "EFFICIENT NETWORKS, INC";
  case 0x111b:    return "LITTON GCS";
  case 0x111c:    return "TRICORD SYSTEMS";
  case 0x111d:    return "INTEGRATED DEVICE TECH";
  case 0x111e:    return "WESTERN DIGITAL";
  case 0x111f:    return "PRECISION DIGITAL IMAGES";
  case 0x1120:    return "EMC CORPORATION";
  case 0x1121:    return "ZILOG";
  case 0x1122:    return "MULTI-TECH SYSTEMS, INC.";
  case 0x1123:    return "EXCELLENT DESIGN, INC.";
  case 0x1124:    return "LEUTRON VISION AG";
  case 0x1127:    return "FORE SYSTEMS INC";
  case 0x1129:    return "FIRMWORKS";
  case 0x112a:    return "HERMES ELECTRONICS COMPANY, LTD.";
  case 0x112b:    return "LINOTYPE - HELL AG";
  case 0x112c:    return "ZENITH DATA SYSTEMS";
  case 0x112d:    return "RAVICAD";
  case 0x112f:    return "IMAGING TECHNOLOGY, INC";
  case 0x1130:    return "COMPUTERVISION";
  case 0x1131:    return "PHILIPS SEMICONDUCTORS";
  case 0x1132:    return "MITEL CORP.";
  case 0x1133:    return "EICON TECHNOLOGY CORPORATION";
  case 0x1134:    return "MERCURY COMPUTER SYSTEMS";
  case 0x1135:    return "FUJI XEROX CO LTD";
  case 0x1136:    return "MOMENTUM DATA SYSTEMS";
  case 0x1137:    return "CISCO SYSTEMS INC";
  case 0x1138:    return "ZIATECH CORPORATION";
  case 0x1139:    return "DYNAMIC PICTURES, INC";
  case 0x113a:    return "FWB INC";
  case 0x113b:    return "NETWORK COMPUTING DEVICES";
  case 0x113c:    return "CYCLONE MICROSYSTEMS, INC.";
  case 0x113d:    return "LEADING EDGE PRODUCTS INC";
  case 0x113e:    return "SANYO ELECTRIC CO";
  case 0x113f:    return "EQUINOX SYSTEMS, INC.";
  case 0x1140:    return "INTERVOICE INC";
  case 0x1141:    return "CREST MICROSYSTEM INC";
  case 0x1142:    return "ALLIANCE SEMICONDUCTOR CORPORATION";
  case 0x1143:    return "NETPOWER, INC";
  case 0x1144:    return "VICKERS, INC.";
  case 0x1145:    return "WORKBIT CORPORATION";
  case 0x1146:    return "FORCE COMPUTERS";
  case 0x1147:    return "INTERFACE CORP";
  case 0x1148:    return "SYSKONNECT";
  case 0x1149:    return "WIN SYSTEM CORPORATION";
  case 0x114a:    return "VMIC";
  case 0x114b:    return "CANOPUS CO., LTD";
  case 0x114c:    return "ANNABOOKS";
  case 0x114d:    return "IC CORPORATION";
  case 0x114e:    return "NIKON SYSTEMS INC";
  case 0x114f:    return "DIGI INTERNATIONAL";
  case 0x1150:    return "THINKING MACHINES CORP";
  case 0x1151:    return "JAE ELECTRONICS INC.";
  case 0x1152:    return "MEGATEK";
  case 0x1153:    return "LAND WIN ELECTRONIC CORP";
  case 0x1154:    return "MELCO INC";
  case 0x1155:    return "PINE TECHNOLOGY LTD";
  case 0x1156:    return "PERISCOPE ENGINEERING";
  case 0x1157:    return "AVSYS CORPORATION";
  case 0x1158:    return "VORAX R & D INC";
  case 0x1159:    return "MUTECH CORP";
  case 0x115a:    return "HARLEQUIN LTD";
  case 0x115b:    return "PARALLAX GRAPHICS";
  case 0x115c:    return "PHOTRON LTD.";
  case 0x115d:    return "Netaccess";
  case 0x115f:    return "MAXTOR CORPORATION";
  case 0x1160:    return "MEGASOFT INC";
  case 0x1161:    return "PFU LIMITED";
  case 0x1162:    return "OA LABORATORY CO LTD";
  case 0x1163:    return "RENDITION";
  case 0x1164:    return "ADVANCED PERIPHERALS TECH";
  case 0x1165:    return "IMAGRAPH CORPORATION";
  case 0x1166:    return "Ross Computer";
  case 0x1167:    return "MUTOH INDUSTRIES INC";
  case 0x1168:    return "THINE ELECTRONICS INC";
  case 0x1169:    return "CENTRE FOR DEV. OF ADVANCED COMPUTING";
  case 0x116a:    return "POLARIS COMMUNICATIONS";
  case 0x116b:    return "CONNECTWARE INC";
  case 0x116c:    return "INTELLIGENT RESOURCES INTEGRATED SYSTEMS";
  case 0x116e:    return "ELECTRONICS FOR IMAGING";
  case 0x116f:    return "WORKSTATION TECHNOLOGY";
  case 0x1170:    return "INVENTEC CORPORATION";
  case 0x1171:    return "LOUGHBOROUGH SOUND IMAGES PLC";
  case 0x1172:    return "ALTERA CORPORATION";
  case 0x1173:    return "ADOBE SYSTEMS, INC";
  case 0x1174:    return "BRIDGEPORT MACHINES";
  case 0x1175:    return "MITRON COMPUTER INC.";
  case 0x1176:    return "SBE INCORPORATED";
  case 0x1177:    return "SILICON ENGINEERING";
  case 0x1178:    return "ALFA, INC.";
  case 0x1179:    return "TOSHIBA AMERICA INFO SYSTEMS";
  case 0x117a:    return "A-TREND TECHNOLOGY";
  case 0x117b:    return "L G ELECTRONICS, INC.";
  case 0x117c:    return "ATTO TECHNOLOGY";
  case 0x117d:    return "BECTON DICKINSON";
  case 0x117e:    return "T/R SYSTEMS";
  case 0x117f:    return "INTEGRATED CIRCUIT SYSTEMS";
  case 0x1180:    return "RICOH CO LTD";
  case 0x1183:    return "FUJIKURA LTD";
  case 0x1184:    return "FORKS INC";
  case 0x1185:    return "DATAWORLD INT'L LTD";
  case 0x1186:    return "D-LINK SYSTEM INC";
  case 0x1187:    return "ADVANCED TECHNOLOGY LABORATORIES, INC.";
  case 0x1188:    return "SHIMA SEIKI MANUFACTURING LTD.";
  case 0x1189:    return "MATSUSHITA ELECTRONICS CO LTD";
  case 0x118a:    return "HILEVEL TECHNOLOGY";
  case 0x118b:    return "HYPERTEC PTY LIMITED";
  case 0x118c:    return "COROLLARY, INC";
  case 0x118d:    return "BITFLOW INC";
  case 0x118e:    return "HERMSTEDT GMBH";
  case 0x118f:    return "GREEN LOGIC";
  case 0x1191:    return "ARTOP ELECTRONIC CORP";
  case 0x1192:    return "DENSAN COMPANY LTD";
  case 0x1193:    return "Cabletron";
  case 0x1194:    return "TOUCAN TECHNOLOGY";
  case 0x1195:    return "RATOC SYSTEM INC";
  case 0x1196:    return "HYTEC ELECTRONICS LTD";
  case 0x1197:    return "GAGE APPLIED SCIENCES, INC.";
  case 0x1198:    return "LAMBDA SYSTEMS INC";
  case 0x1199:    return "ATTACHMATE CORPORATION";
  case 0x119a:    return "MIND SHARE, INC.";
  case 0x119b:    return "OMEGA MICRO INC.";
  case 0x119c:    return "INFORMATION TECHNOLOGY INST.";
  case 0x119d:    return "BUG, INC.";
  case 0x119e:    return "FUJITSU MICROELECTRONICS LTD.";
  case 0x119f:    return "BULL HN INFORMATION SYSTEMS";
  case 0x11a1:    return "HAMAMATSU PHOTONICS K.K.";
  case 0x11a2:    return "SIERRA RESEARCH AND TECHNOLOGY";
  case 0x11a3:    return "DEURETZBACHER GMBH & CO. ENG. KG";
  case 0x11a4:    return "BARCO GRAPHICS NV";
  case 0x11a5:    return "MICROUNITY SYSTEMS ENG. INC";
  case 0x11a6:    return "PURE DATA LTD.";
  case 0x11a7:    return "POWER COMPUTING CORP.";
  case 0x11a8:    return "SYSTECH CORP.";
  case 0x11a9:    return "INNOSYS";
  case 0x11aa:    return "ACTEL";
  case 0x11ab:    return "GALILEO TECHNOLOGY LTD.";
  case 0x11ac:    return "CANON INFO. SYS. RESEARCH AUST.";
  case 0x11ad:    return "LITE-ON COMMUNICATIONS INC";
  case 0x11ae:    return "SCITEX CORPORATION";
  case 0x11af:    return "PRO-LOG CORPORATION";
  case 0x11b0:    return "V3 SEMICONDUCTOR INC.";
  case 0x11b1:    return "APRICOT COMPUTERS";
  case 0x11b2:    return "EASTMAN KODAK";
  case 0x11b3:    return "BARR SYSTEMS INC.";
  case 0x11b4:    return "LEITCH TECHNOLOGY INTERNATIONAL";
  case 0x11b5:    return "RADSTONE TECHNOLOGY PLC";
  case 0x11b6:    return "UNITED VIDEO CORP";
  case 0x11b8:    return "XPOINT TECHNOLOGIES, INC";
  case 0x11b9:    return "PATHLIGHT TECHNOLOGY INC";
  case 0x11ba:    return "VIDEOTRON CORP";
  case 0x11bb:    return "PYRAMID TECHNOLOGY";
  case 0x11bc:    return "NETWORK PERIPHERALS INC";
  case 0x11bd:    return "PINNACLE SYSTEMS INC.";
  case 0x11be:    return "INTERNATIONAL MICROCIRCUITS INC";
  case 0x11bf:    return "ASTRODESIGN, INC.";
  case 0x11c1:    return "AT&T MICROELECTRONICS";
  case 0x11c2:    return "SAND MICROELECTRONICS";
  case 0x11c4:    return "DOCUMENT TECHNOLOGIES, INC";
  case 0x11c5:    return "SHIVA CORPORATION";
  case 0x11c6:    return "DAINIPPON SCREEN MFG. CO. LTD";
  case 0x11c7:    return "D.C.M. DATA SYSTEMS";
  case 0x11c8:    return "DOLPHIN INTERCONNECT SOLUTIONS AS";
  case 0x11c9:    return "MAGMA";
  case 0x11ca:    return "LSI SYSTEMS, INC";
  case 0x11cb:    return "SPECIALIX RESEARCH LTD";
  case 0x11cc:    return "MICHELS & KLEBERHOFF COMPUTER GMBH";
  case 0x11cd:    return "HAL COMPUTER SYSTEMS, INC.";
  case 0x11ce:    return "Netaccess";
  case 0x11cf:    return "PIONEER ELECTRONIC CORPORATION";
  case 0x11d0:    return "LOCKHEED MARTIN FEDERAL SYSTEMS-MANASSAS";
  case 0x11d1:    return "AURAVISION";
  case 0x11d2:    return "INTERCOM INC.";
  case 0x11d3:    return "TRANCELL SYSTEMS INC";
  case 0x11d4:    return "ANALOG DEVICES";
  case 0x11d5:    return "IKON CORPORATION";
  case 0x11d6:    return "TEKELEC TELECOM";
  case 0x11d7:    return "TRENTON TECHNOLOGY, INC.";
  case 0x11d8:    return "IMAGE TECHNOLOGIES DEVELOPMENT";
  case 0x11d9:    return "TEC CORPORATION";
  case 0x11da:    return "NOVELL";
  case 0x11db:    return "SEGA ENTERPRISES LTD";
  case 0x11dc:    return "QUESTRA CORPORATION";
  case 0x11dd:    return "CROSFIELD ELECTRONICS LIMITED";
  case 0x11de:    return "ZORAN CORPORATION";
  case 0x11df:    return "NEW WAVE PDG";
  case 0x11e1:    return "GEC PLESSEY SEMI INC.";
  case 0x11e2:    return "SAMSUNG INFORMATION SYSTEMS AMERICA";
  case 0x11e3:    return "QUICKLOGIC CORPORATION";
  case 0x11e4:    return "SECOND WAVE INC";
  case 0x11e5:    return "IIX CONSULTING";
  case 0x11e6:    return "MITSUI-ZOSEN SYSTEM RESEARCH";
  case 0x11e8:    return "DIGITAL PROCESSING SYSTEMS INC.";
  case 0x11ea:    return "ELSAG BAILEY";
  case 0x11eb:    return "FORMATION INC.";
  case 0x11ec:    return "CORECO INC";
  case 0x11ed:    return "MEDIAMATICS";
  case 0x11ee:    return "DOME IMAGING SYSTEMS INC";
  case 0x11ef:    return "NICOLET TECHNOLOGIES B.V.";
  case 0x11f0:    return "COMPU-SHACK GMBH";
  case 0x11f1:    return "SYMBIOS LOGIC INC";
  case 0x11f2:    return "PICTURE TEL JAPAN K.K.";
  case 0x11f3:    return "KEITHLEY METRABYTE";
  case 0x11f4:    return "KINETIC SYSTEMS CORPORATION";
  case 0x11f5:    return "COMPUTING DEVICES INTERNATIONAL";
  case 0x11f6:    return "POWERMATIC DATA SYSTEMS LTD";
  case 0x11f7:    return "SCIENTIFIC ATLANTA";
  case 0x11f8:    return "PMC-SIERRA INC";
  case 0x11f9:    return "I-CUBE INC";
  case 0x11fa:    return "KASAN ELECTRONICS COMPANY, LTD.";
  case 0x11fb:    return "DATEL INC";
  case 0x11fd:    return "HIGH STREET CONSULTANTS";
  case 0x11fe:    return "COMTROL CORPORATION";
  case 0x11ff:    return "SCION CORPORATION";
  case 0x1200:    return "CSS CORPORATION";
  case 0x1201:    return "VISTA CONTROLS CORP";
  case 0x1202:    return "NETWORK GENERAL CORP.";
  case 0x1203:    return "BAYER CORPORATION, AGFA DIVISION";
  case 0x1204:    return "LATTICE SEMICONDUCTOR CORPORATION";
  case 0x1205:    return "ARRAY CORPORATION";
  case 0x1206:    return "AMDAHL CORPORATION";
  case 0x1208:    return "PARSYTEC GMBH";
  case 0x1209:    return "SCI SYSTEMS INC";
  case 0x120a:    return "SYNAPTEL";
  case 0x120b:    return "ADAPTIVE SOLUTIONS";
  case 0x120d:    return "COMPRESSION LABS, INC.";
  case 0x120e:    return "CYCLADES CORPORATION";
  case 0x120f:    return "ESSENTIAL COMMUNICATIONS";
  case 0x1210:    return "HYPERPARALLEL TECHNOLOGIES";
  case 0x1211:    return "BRAINTECH INC";
  case 0x1212:    return "KINGSTON TECHNOLOGY CORP.";
  case 0x1213:    return "APPLIED INTELLIGENT SYSTEMS, INC.";
  case 0x1214:    return "PERFORMANCE TECHNOLOGIES, INC.";
  case 0x1215:    return "INTERWARE CO., LTD";
  case 0x1216:    return "PURUP PREPRESS A/S";
  case 0x1217:    return "02 MICRO, INC.";
  case 0x1218:    return "HYBRICON CORP.";
  case 0x1219:    return "FIRST VIRTUAL CORPORATION";
  case 0x121a:    return "3DFX INTERACTIVE, INC.";
  case 0x121b:    return "ADVANCED TELECOMM MODULES";
  case 0x121c:    return "NIPPON TEXACO, LTD.";
  case 0x121d:    return "LIPPERT AUTOMATIONSTECHNIK GMBH";
  case 0x121e:    return "CSPI";
  case 0x121f:    return "ARCUS TECHNOLOGY, INC.";
  case 0x1220:    return "ARIEL CORPORATION";
  case 0x1221:    return "CONTEC CO., LTD";
  case 0x1222:    return "ANCOR COMMUNICATIONS, INC.";
  case 0x1223:    return "HEURIKON/COMPUTER PRODUCTS";
  case 0x1224:    return "INTERACTIVE IMAGES";
  case 0x1225:    return "POWER I/O, INC.";
  case 0x1227:    return "TECH-SOURCE";
  case 0x1228:    return "NORSK ELEKTRO OPTIKK A/S";
  case 0x1229:    return "DATA KINESIS INC.";
  case 0x122a:    return "INTEGRATED TELECOM";
  case 0x122b:    return "LG INDUSTRIAL SYSTEMS CO., LTD";
  case 0x122c:    return "SICAN GMBH";
  case 0x122d:    return "AZTECH SYSTEM LTD";
  case 0x122e:    return "XYRATEX";
  case 0x122f:    return "ANDREW CORPORATION";
  case 0x1230:    return "FISHCAMP ENGINEERING";
  case 0x1231:    return "WOODWARD McCOACH, INC.";
  case 0x1232:    return "GPT LIMITED";
  case 0x1233:    return "BUS-TECH, INC.";
  case 0x1234:    return "TECHNICAL CORP.";
  case 0x1235:    return "RISQ MODULAR SYSTEMS, INC.";
  case 0x1236:    return "Sigma Designs Corporation";
  case 0x1237:    return "ALTA TECHNOLOGY CORPORATION";
  case 0x1238:    return "ADTRAN";
  case 0x1239:    return "3DO COMPANY";
  case 0x123a:    return "VISICOM LABORATORIES, INC.";
  case 0x123b:    return "SEEQ TECHNOLOGY, INC.";
  case 0x123c:    return "CENTURY SYSTEMS, INC.";
  case 0x123d:    return "ENGINEERING DESIGN TEAM, INC.";
  case 0x123e:    return "SIMUTECH, INC.";
  case 0x123f:    return "C-CUBE MICROSYSTEMS";
  case 0x1240:    return "MARATHON TECHNOLOGIES CORP.";
  case 0x1241:    return "DSC COMMUNICATIONS";
  case 0x1243:    return "DELPHAX";
  case 0x1244:    return "AVM AUDIOVISUELLES MKTG & COMPUTER GMBH";
  case 0x1245:    return "A.P.D., S.A.";
  case 0x1246:    return "DIPIX TECHNOLOGIES, INC.";
  case 0x1247:    return "XYLON RESEARCH, INC.";
  case 0x1248:    return "CENTRAL DATA CORPORATION";
  case 0x1249:    return "SAMSUNG ELECTRONICS CO., LTD.";
  case 0x124a:    return "AEG ELECTROCOM GMBH";
  case 0x124b:    return "GREENSPRING COMPUTERS INC.";
  case 0x124c:    return "SOLITRON TECHNOLOGIES, INC.";
  case 0x124d:    return "STALLION TECHNOLOGIES, INC.";
  case 0x124e:    return "CYLINK";
  case 0x124f:    return "INFORTREND TECHNOLOGY, INC.";
  case 0x1250:    return "HITACHI MICROCOMPUTER SYSTEM LTD";
  case 0x1251:    return "VLSI SOLUTION OY";
  case 0x1253:    return "GUZIK TECHNICAL ENTERPRISES";
  case 0x1254:    return "LINEAR SYSTEMS LTD.";
  case 0x1255:    return "OPTIBASE LTD";
  case 0x1256:    return "PERCEPTIVE SOLUTIONS, INC.";
  case 0x1257:    return "VERTEX NETWORKS, INC.";
  case 0x1258:    return "GILBARCO, INC.";
  case 0x1259:    return "ALLIED TELESYN INTERNATIONAL";
  case 0x125a:    return "ABB POWER SYSTEMS";
  case 0x125b:    return "ASIX ELECTRONICS CORPORATION";
  case 0x125c:    return "AURORA TECHNOLOGIES, INC.";
  case 0x125d:    return "ESS TECHNOLOGY";
  case 0x125e:    return "SPECIALVIDEO ENGINEERING SRL";
  case 0x125f:    return "CONCURRENT TECHNOLOGIES, INC.";
  case 0x1260:    return "HARRIS SEMICONDUCTOR";
  case 0x1261:    return "MATSUSHITA-KOTOBUKI ELECTRONICS INDUSTRIES, ";
  case 0x1262:    return "ES COMPUTER COMPANY, LTD.";
  case 0x1263:    return "SONIC SOLUTIONS";
  case 0x1264:    return "AVAL NAGASAKI CORPORATION";
  case 0x1265:    return "CASIO COMPUTER CO., LTD.";
  case 0x1266:    return "MICRODYNE CORPORATION";
  case 0x1267:    return "S. A. TELECOMMUNICATIONS";
  case 0x1268:    return "TEKTRONIX";
  case 0x1269:    return "THOMSON-CSF/TTM";
  case 0x126a:    return "LEXMARK INTERNATIONAL, INC.";
  case 0x126b:    return "ADAX, INC.";
  case 0x126c:    return "NORTHERN TELECOM";
  case 0x126d:    return "SPLASH TECHNOLOGY, INC.";
  case 0x126e:    return "SUMITOMO METAL INDUSTRIES, LTD.";
  case 0x126f:    return "SILICON MOTION, INC.";
  case 0x1270:    return "OLYMPUS OPTICAL CO., LTD.";
  case 0x1271:    return "GW INSTRUMENTS";
  case 0x1272:    return "TELEMATICS";
  case 0x1273:    return "HUGHES NETWORK SYSTEMS";
  case 0x1274:    return "ENSONIQ";
  case 0x1275:    return "NETWORK APPLIANCE CORPORATION";
  case 0x1276:    return "SWITCHED NETWORK TECHNOLOGIES, INC.";
  case 0x1277:    return "COMSTREAM";
  case 0x1278:    return "TRANSTECH PARALLEL SYSTEMS LTD.";
  case 0x1279:    return "TRANSMETA CORPORATION";
  case 0x127a:    return "ROCKWELL INTERNATIONAL";
  case 0x127b:    return "PIXERA CORPORATION";
  case 0x127c:    return "CROSSPOINT SOLUTIONS, INC.";
  case 0x127d:    return "VELA RESEARCH";
  case 0x127e:    return "WINNOV, L.P.";
  case 0x127f:    return "FUJIFILM";
  case 0x1280:    return "PHOTOSCRIPT GROUP LTD.";
  case 0x1281:    return "YOKOGAWA ELECTRIC CORPORATION";
  case 0x1282:    return "DAVICOM SEMICONDUCTOR, INC.";
  case 0x1283:    return "INTEGRATED TECHNOLOGY EXPRESS, INC.";
  case 0x1284:    return "SAHARA NETWORKS, INC.";
  case 0x1285:    return "PLATFORM TECHNOLOGIES, INC.";
  case 0x1286:    return "MAZET GMBH";
  case 0x1287:    return "M-PACT, INC.";
  case 0x1288:    return "TIMESTEP CORPORATION";
  case 0x1289:    return "AVC TECHNOLOGY, INC.";
  case 0x128a:    return "ASANTE TECHNOLOGIES, INC.";
  case 0x128b:    return "TRANSWITCH CORPORATION";
  case 0x128c:    return "RETIX CORPORATION";
  case 0x128d:    return "G2 NETWORKS, INC.";
  case 0x128e:    return "SAMHO MULTI TECH LTD.";
  case 0x128f:    return "TATENO DENNOU, INC.";
  case 0x1290:    return "SORD COMPUTER CORPORATION";
  case 0x1291:    return "NCS COMPUTER ITALIA";
  case 0x1292:    return "TRITECH MICROELECTRONICS  INTERNATIONAL PTE. ";
  case 0x1293:    return "MEDIA REALITY TECHNOLOGY";
  case 0x1294:    return "RHETOREX, INC.";
  case 0x1295:    return "IMAGENATION CORPORATION";
  case 0x1296:    return "KOFAX IMAGE PRODUCTS";
  case 0x1297:    return "HOLCO ENT CO, LTD/SHUTTLE COMPUTER";
  case 0x1298:    return "SPELLCASTER TELECOMMUNICATIONS INC.";
  case 0x1299:    return "KNOWLEDGE TECHNOLOGY LAB.";
  case 0x129a:    return "VMETRO, INC.";
  case 0x129b:    return "IMAGE ACCESS";
  case 0x129c:    return "JAYCOR";
  case 0x129d:    return "COMPCORE MULTIMEDIA, INC.";
  case 0x129e:    return "VICTOR COMPANY OF JAPAN, LTD.";
  case 0x129f:    return "OEC MEDICAL SYSTEMS, INC.";
  case 0x12a0:    return "ALLEN- BRADLEY COMPANY";
  case 0x12a1:    return "SIMPACT ASSOCIATES, INC.";
  case 0x12a2:    return "NEWGEN SYSTEMS CORPORATION";
  case 0x12a3:    return "LUCENT TECHNOLOGIES";
  case 0x12a4:    return "NTT ELECTRONICS TECHNOLOGY COMPANY";
  case 0x12a5:    return "VISION DYNAMICS LTD.";
  case 0x12a6:    return "SCALABLE NETWORKS, INC.";
  case 0x12a7:    return "AMO GMBH";
  case 0x12a8:    return "NEWS DATACOM";
  case 0x12a9:    return "XIOTECH CORPORATION";
  case 0x12aa:    return "SDL COMMUNIATIONS, INC.";
  case 0x12ab:    return "YUAN YUAN ENTERPRISE CO., LTD.";
  case 0x12ac:    return "MEASUREX CORPORATION";
  case 0x12ad:    return "MULTIDATA GMBH";
  case 0x12ae:    return "Alteon Networks Inc.";
  case 0x12af:    return "TDK USA Corp";
  case 0x12b0:    return "Jorge Scientific Corp";
  case 0x12b1:    return "GammaLink";
  case 0x12b2:    return "General Signal Networks";
  case 0x12b3:    return "Inter-Face Co Ltd";
  case 0x12b4:    return "FutureTel Inc";
  case 0x12b5:    return "Granite Systems Inc.";
  case 0x12b6:    return "NATURAL MICROSYSTEMS";
  case 0x12b7:    return "ACUMEN";
  case 0x12b8:    return "Korg";
  case 0x12b9:    return "US Robotics";
  case 0x12ba:    return "PMC Sierra";
  case 0x12bb:    return "NIPPON UNISOFT CORPORATION";
  case 0x12bc:    return "ARRAY MICROSYSTEMS";
  case 0x12bd:    return "COMPUTERM CORP.";
  case 0x12be:    return "Anchor Chips";
  case 0x12bf:    return "Fujifilm Microdevices";
  case 0x12c0:    return "INFIMED";
  case 0x12c1:    return "GMM Research Corp";
  case 0x12c2:    return "Mentec Limited";
  case 0x12c3:    return "Holtek Microelectronics Inc";
  case 0x12c4:    return "Connect Tech Inc";
  case 0x12c5:    return "PICTURE ELEMENTS";
  case 0x12c6:    return "Mitani Corporation";
  case 0x12c7:    return "Dialogic Corp";
  case 0x12c8:    return "G Force Co, Ltd";
  case 0x12c9:    return "Gigi Operations";
  case 0x12ca:    return "Integrated Computing Engines";
  case 0x12cb:    return "ANTEX ELECTRONICS CORPORATION";
  case 0x12cc:    return "PLUTO TECHNOLOGIES INTERNATIONAL";
  case 0x12cd:    return "Aims Lab";
  case 0x12ce:    return "NETSPEED INC.";
  case 0x12cf:    return "PROPHET SYSTEMS, INC.";
  case 0x12d0:    return "GDE SYSTEMS, INC.";
  case 0x12d1:    return "PSITECH";
  case 0x12d2:    return "Nvidia/SGS Thomson";
  case 0x12d3:    return "Vingmed Sound A/S";
  case 0x12d4:    return "DGM&S";
  case 0x12d5:    return "Equator Technologies";
  case 0x12d6:    return "Analogic Corp";
  case 0x12d7:    return "Biotronic SRL";
  case 0x12d8:    return "PERICOM SEMICONDUCTOR";
  case 0x12d9:    return "Aculab PLC";
  case 0x12da:    return "True Time";
  case 0x12db:    return "Annapolis Micro Systems, Inc";
  case 0x12dc:    return "Symicron Computer Connumiaction Ltd.";
  case 0x12dd:    return "Management Graphics";
  case 0x12de:    return "Rainbow Technologies";
  case 0x12df:    return "SBS Technologies Inc";
  case 0x12e0:    return "Chase Research";
  case 0x12e1:    return "Nintendo Co, Ltd";
  case 0x12e2:    return "Datum Inc. Bancomm-Timing Division";
  case 0x12e3:    return "Imation Corp - Medical Imaging Systems";
  case 0x12e4:    return "Brooktrout Technology Inc";
  case 0x12e5:    return "Apex Inc";
  case 0x12e6:    return "Cirel Systems";
  case 0x12e7:    return "SUNSGROUP CORPORATION";
  case 0x12e8:    return "CRISC Corp";
  case 0x12e9:    return "GE Spacenet";
  case 0x12ea:    return "Zuken";
  case 0x12eb:    return "AUREAL SEMICONDUCTOR";
  case 0x12ec:    return "3A INTERNATIONAL, INC.";
  case 0x12ed:    return "Optivision Inc.";
  case 0x12ee:    return "Orange Micro";
  case 0x12ef:    return "Vienna Systems";
  case 0x12f0:    return "PENTEK";
  case 0x12f1:    return "Sorenson Vision Inc";
  case 0x12f2:    return "GAMMAGRAPHX, INC.";
  case 0x12f3:    return "XING INC.";
  case 0x12f4:    return "Megatel";
  case 0x12f5:    return "Forks";
  case 0x12f6:    return "Dawson France";
  case 0x12f7:    return "COGNEX";
  case 0x3d3d:    return "3DLABS LIMITED";
  case 0x3d:      return "LOCKHEED MARTIN";
  case 0x4005:    return "AVANCE LOGIC INC";
  case 0x4680:    return "UMAX Computer Corp";
  case 0x4754:    return "TRITECH MICROELECTRONICS";
  case 0x5333:    return "S3 INC.";
  case 0x8086:    return "Intel Corp.";
  case 0x8888:    return "SILICON MAGIC";
  case 0x8e0e:    return "COMPUTONE CORPORATION";
  case 0x9004:    return "ADAPTEC";
  case 0xc0fe:    return "MOTION ENGINEERING, INC.";
  case 0xe11:     return "COMPAQ COMPUTER CORP.";
  case 0xedd8:    return "ARK LOGIC INC";
  default:                return "???";
  }
} /* end pciGetVendorName() */

