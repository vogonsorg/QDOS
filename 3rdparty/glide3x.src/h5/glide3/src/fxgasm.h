
;----------------------------------------------------------------------
; Assembler offsets for SSTREGS struct
;----------------------------------------------------------------------



;----------------------------------------------------------------------
; Assembler offsets for GC struct
;----------------------------------------------------------------------

base_ptr		equ 00000074h
reg_ptr			equ 00000078h
lfb_ptr			equ 00000080h
cull_mode		equ 000001e0h
tsuDataList		equ 00000124h
triPacketHdr	equ 00000e00h
cullStripHdr	equ 00000e04h
paramMask	equ 00000e08h
fifoStart	equ 00000e2ch
fifoEnd	equ 00000e30h
fifoOffset	equ 00000e34h
fifoSize	equ 00000e38h
fifoJmpHdr	equ 00000e3ch
fifoPtr	equ 00000e0ch
fifoRead	equ 00000e10h
fifoRoom	equ 00000e14h
roomToReadPtr	equ 00000e44h
roomToEnd	equ 00000e48h
lfbLockCount	equ 00000e4ch
triSetupProc	equ 00000dech
drawTrianglesProc	equ 00000df0h
drawVertexList	equ 00000dech
vertexSize	equ 00000b88h
vertexStride	equ 00000b84h
invalid	equ 00000b90h
CoordinateSpace	equ 00000dbch
paramIndex	equ 000001e4h
vp_hwidth	equ 00000b18h
vp_hheight	equ 00000b1ch
vp_hdepth	equ 00000b20h
vp_ox	equ 00000b0ch
vp_oy	equ 00000b10h
vp_oz	equ 00000b14h
colorType	equ 00000b8ch
wInfo_offset	equ 00000b38h
qInfo_mode	equ 00000b6ch
qInfo_offset	equ 00000b70h
q0Info_offset	equ 00000b78h
q1Info_offset	equ 00000b80h
q0Info_mode	equ 00000b74h
q1Info_mode	equ 00000b7ch
fogInfo_offset	equ 00000b48h
fogInfo_mode	equ 00000b44h
depth_range	equ 00000a5ch
tmu0_s_scale	equ 00000a0ch
tmu0_t_scale	equ 00000a10h
tmu1_s_scale	equ 00000a34h
tmu1_t_scale	equ 00000a38h
fbi_fbzMode	equ 00000210h
curTriSize		equ 0000004ch
trisProcessed		equ 0000000ch
trisDrawn		equ 00000010h
lostContext		equ 000096e4h
windowed		equ 000096ech
bInfo		equ 0000011ch

;----------------------------------------------------------------------
; Assembler offsets for GlideRoot struct
;----------------------------------------------------------------------

p6Fencer		equ 00000000h
current_sst		equ 0000000ch
CPUType			equ 00000020h
tlsOffset			equ 00000008h
pool_f255	equ 00000054h
pool_f1	equ 00000050h
SIZEOF_GrState		equ 00000be4h
SIZEOF_GC		equ 00009738h

SIZEOF_GlideRoot	equ 000bd618h

