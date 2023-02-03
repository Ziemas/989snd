#ifndef TYPES_H_
#define TYPES_H_
#include "stdbool.h"
#include "stddef.h"

#include <cdvdman.h>
#include <sifman.h>
#include <sifrpc.h>
#include <thbase.h>
#include <thsemap.h>

typedef iop_sys_clock_t SysClock;
typedef iop_thread_t ThreadParam;
typedef iop_thread_info_t ThreadInfo;
typedef iop_sema_t SemaParam;
typedef SifRpcServerData_t sceSifServeData;
typedef SifRpcReceiveData_t sceSifReceiveData;
typedef SifRpcDataQueue_t sceSifQueueData;
typedef SifDmaTransfer_t sceSifDmaData;

typedef char SInt8;
typedef unsigned char UInt8;
typedef short int SInt16;
typedef short unsigned int UInt16;
typedef int SInt32;
typedef unsigned int UInt32;
typedef unsigned int u_int;
typedef long int SInt64;
typedef long unsigned int UInt64;
typedef long unsigned int u_long;

typedef struct { // 0x4
    /* 0x0 */ SInt16 left;
    /* 0x2 */ SInt16 right;
} SpuVolume;

typedef struct { // 0x4
    /* 0x0 */ UInt16 adsr1;
    /* 0x2 */ UInt16 adsr2;
} ADSRSettings;

struct VoiceFlags { // 0x8
    /* 0x0 */ UInt32 core[2];
};

typedef struct { // 0x30
    /* 0x00 */ UInt32 ID;
    /* 0x04 */ UInt32 Version;
    /* 0x08 */ UInt32 Reserved1;
    /* 0x0c */ UInt32 DataSize;
    /* 0x10 */ UInt32 SampleRate;
    /* 0x14 */ UInt32 Reserved2;
    /* 0x18 */ UInt32 Reserved3;
    /* 0x1c */ UInt32 Reserved4;
    /* 0x20 */ char Name[16];
} SCEIVAGHeader;

enum ToneFlags {
    TONE_REVERB = 1 << 1,
    TONE_REVERB_ONLY = 1 << 4,
};

typedef struct { // 0x18
    /* 0x00 */ SInt8 Priority;
    /* 0x01 */ SInt8 Vol;
    /* 0x02 */ SInt8 CenterNote;
    /* 0x03 */ SInt8 CenterFine;
    /* 0x04 */ SInt16 Pan;
    /* 0x06 */ SInt8 MapLow;
    /* 0x07 */ SInt8 MapHigh;
    /* 0x08 */ SInt8 PBLow;
    /* 0x09 */ SInt8 PBHigh;
    /* 0x0a */ UInt16 ADSR1;
    /* 0x0c */ UInt16 ADSR2;
    /* 0x0e */ SInt16 Flags;
    /* 0x10 */ void *VAGInSR;
    /* 0x14 */ UInt32 reserved1;
} Tone;

typedef Tone *TonePtr;

typedef struct { // 0x8
    /* 0x0 */ SInt8 NumTones;
    /* 0x1 */ SInt8 Vol;
    /* 0x2 */ SInt16 Pan;
    /* 0x4 */ TonePtr FirstTone;
} Prog;

typedef Prog *ProgPtr;

struct SoundBank_tag;

typedef struct { // 0x1c
    /* 0x00 */ SInt32 Type;
    /* 0x04 */ struct SoundBank_tag *Bank;
    /* 0x08 */ void *OrigBank;
    /* 0x0c */ SInt8 OrigType;
    /* 0x0d */ SInt8 Prog;
    /* 0x0e */ SInt8 Note;
    /* 0x0f */ SInt8 Fine;
    /* 0x10 */ SInt16 Vol;
    /* 0x12 */ SInt8 pad1;
    /* 0x13 */ SInt8 VolGroup;
    /* 0x14 */ SInt16 Pan;
    /* 0x16 */ SInt8 XREFSound;
    /* 0x17 */ SInt8 pad2;
    /* 0x18 */ UInt16 Flags;
    /* 0x1a */ UInt16 pad3;
} Sound;

typedef Sound *SoundPtr;

typedef struct { // 0x1c
    /* 0x00 */ SInt32 Type;
    /* 0x04 */ struct SoundBank_tag *Bank;
    /* 0x08 */ SInt8 IDs[20];
} CompoundSound;

typedef CompoundSound *CompoundSoundPtr;

typedef struct { // 0x1c
    /* 0x00 */ SInt32 Type;
    /* 0x04 */ struct SoundBank_tag *Bank;
    /* 0x08 */ void *OrigBank;
    /* 0x0c */ UInt32 MIDIID;
    /* 0x10 */ SInt16 Vol;
    /* 0x12 */ SInt8 Repeats;
    /* 0x13 */ SInt8 VolGroup;
    /* 0x14 */ SInt16 Pan;
    /* 0x16 */ SInt8 Index;
    /* 0x17 */ UInt8 Flags;
    /* 0x18 */ void *MIDIBlock;
} MIDISound;

typedef MIDISound *MIDISoundPtr;

struct SoundBank_tag { // 0x34
    /* 0x00 */ UInt32 DataID;
    /* 0x04 */ UInt32 Version;
    /* 0x08 */ UInt32 Flags;
    /* 0x0c */ UInt32 BankID;
    /* 0x10 */ SInt8 BankNum;
    /* 0x11 */ SInt8 pad1;
    /* 0x12 */ SInt16 pad2;
    /* 0x14 */ SInt16 NumSounds;
    /* 0x16 */ SInt16 NumProgs;
    /* 0x18 */ SInt16 NumTones;
    /* 0x1a */ SInt16 NumVAGs;
    /* 0x1c */ SoundPtr FirstSound;
    /* 0x20 */ ProgPtr FirstProg;
    /* 0x24 */ TonePtr FirstTone;
    /* 0x28 */ void *VagsInSR;
    /* 0x2c */ UInt32 VagDataSize;
    /* 0x30 */ struct SoundBank_tag *NextBank;
};

typedef struct SoundBank_tag SoundBank;
typedef struct SoundBank_tag *SoundBankPtr;

struct LFOTracker { // 0x30
    /* 0x00 */ UInt16 type;
    /* 0x02 */ UInt8 target;
    /* 0x03 */ UInt8 target_extra;
    /* 0x04 */ UInt8 setup_flags;
    /* 0x05 */ UInt8 running_flags;
    /* 0x06 */ UInt16 depth;
    /* 0x08 */ UInt32 orig_depth;
    /* 0x0c */ SInt32 next_step;
    /* 0x10 */ UInt32 step_size;
    /* 0x14 */ UInt32 orig_step_size;
    /* 0x18 */ SInt32 state_hold1;
    /* 0x1c */ SInt32 state_hold2;
    /* 0x20 */ SInt32 range;
    /* 0x24 */ SInt32 last_lfo;
    /* 0x28 */ struct BlockSoundHandler *handler;
    /* 0x2c */ struct LFOTracker *next;
};

typedef struct { // 0x20
    /* 0x00 */ char blank[32];
} LargestGrainParamStruct;

typedef struct { // 0x10
    /* 0x0 */ UInt32 BankID;
    /* 0x4 */ UInt32 SoundIndex;
    /* 0x8 */ SInt32 PitchMod;
    /* 0xc */ UInt32 Flags;
} XREFGrainParams;

typedef struct { // 0x4
    /* 0x0 */ SInt32 Amount;
} RandDelayParams;

typedef struct { // 0x20
    /* 0x00 */ UInt32 id;
    /* 0x04 */ UInt32 index;
    /* 0x08 */ UInt8 data[24];
} PluginParams;

typedef struct { // 0x10
    /* 0x0 */ UInt8 which_lfo;
    /* 0x1 */ UInt8 target;
    /* 0x2 */ UInt8 target_extra;
    /* 0x3 */ UInt8 shape;
    /* 0x4 */ UInt16 duty_cycle;
    /* 0x6 */ UInt16 depth;
    /* 0x8 */ UInt16 flags;
    /* 0xa */ UInt16 start_offset;
    /* 0xc */ UInt32 step_size;
} LFOParams;

typedef struct { // 0x8
    /* 0x0 */ SInt16 param[4];
} ControlParams;

typedef struct { // 0x20
    /* 0x00 */ SInt32 vol;
    /* 0x04 */ SInt32 pan;
    /* 0x08 */ SInt8 reg_settings[4];
    /* 0x0c */ SInt32 sound_id;
    /* 0x10 */ char snd_name[16];
} PlaySoundParams;

union GrainParams { // 0x20
    /* 0x00 */ Tone tone;
    /* 0x00 */ XREFGrainParams xref;
    /* 0x00 */ RandDelayParams delay;
    /* 0x00 */ ControlParams control;
    /* 0x00 */ LFOParams lfo;
    /* 0x00 */ PlaySoundParams play_sound;
    /* 0x00 */ PluginParams plugin_params;
    /* 0x00 */ LargestGrainParamStruct junk;
};

typedef struct { // 0x28
    /* 0x00 */ UInt32 Type;
    /* 0x04 */ SInt32 Delay;
    /* 0x08 */ union GrainParams Params;
} SFXGrain;

typedef SFXGrain *SFXGrainPtr;

struct _opd1 { // 0x4
    /* 0x0 */ SInt8 Arg[3];
    /* 0x3 */ UInt8 Type;
};

union _op_data { // 0x4
    /* 0x0 */ struct _opd1 MicroOp;
    /* 0x0 */ UInt32 Opcode;
};

typedef struct { // 0x8
    /* 0x0 */ union _op_data OpcodeData;
    /* 0x4 */ SInt32 Delay;
} SFXGrain2;

typedef SFXGrain2 *SFXGrain2Ptr;

typedef struct { // 0xc
    /* 0x0 */ SInt8 Vol;
    /* 0x1 */ SInt8 VolGroup;
    /* 0x2 */ SInt16 Pan;
    /* 0x4 */ SInt8 NumGrains;
    /* 0x5 */ SInt8 InstanceLimit;
    /* 0x6 */ UInt16 Flags;
    /* 0x8 */ SFXGrainPtr FirstGrain;
} SFX;

typedef SFX *SFXPtr;

typedef struct { // 0xc
    /* 0x0 */ SInt8 Vol;
    /* 0x1 */ SInt8 VolGroup;
    /* 0x2 */ SInt16 Pan;
    /* 0x4 */ SInt8 NumGrains;
    /* 0x5 */ SInt8 InstanceLimit;
    /* 0x6 */ UInt16 Flags;
    /* 0x8 */ SFXGrain2Ptr FirstGrain;
} SFX2;

typedef SFX2 *SFX2Ptr;

struct SFXUserData { // 0x10
    /* 0x0 */ UInt32 data[4];
};

typedef struct SFXUserData *SFXUserDatPtr;

struct SFXName { // 0x14
    /* 0x00 */ UInt32 Name[4];
    /* 0x10 */ SInt16 Index;
    /* 0x12 */ SInt16 reserved;
};

typedef struct SFXName *SFXNamePtr;

struct VAGName { // 0x1c
    /* 0x00 */ UInt32 Name[4];
    /* 0x10 */ UInt32 Offset;
    /* 0x14 */ UInt32 res1;
    /* 0x18 */ UInt32 res2;
};

typedef struct VAGName *VAGNamePtr;

struct VAGImport { // 0x20
    /* 0x00 */ UInt32 BlockName[2];
    /* 0x08 */ UInt32 VAGName[4];
    /* 0x18 */ UInt32 VAGLocation;
    /* 0x1c */ UInt32 VAGSR;
};

typedef struct VAGImport *VAGImportPtr;

struct VAGExport { // 0x18
    /* 0x00 */ UInt32 VAGName[4];
    /* 0x10 */ UInt32 VAGLocation;
    /* 0x14 */ UInt32 VAGSR;
};

typedef struct VAGExport *VAGExportPtr;

struct SFXBlockNames { // 0x98
    /* 0x00 */ UInt32 BlockName[2];
    /* 0x08 */ UInt32 SFXNameTableOffset;
    /* 0x0c */ UInt32 VAGNameTableOffset;
    /* 0x10 */ UInt32 VAGImportsTableOffset;
    /* 0x14 */ UInt32 VAGExportsTableOffset;
    /* 0x18 */ SInt16 SFXHashOffsets[32];
    /* 0x58 */ SInt16 VAGHashOffsets[32];
};

typedef struct SFXBlockNames *SFXBlockNamesPtr;

struct SNDModelNodeHeader_t { // 0x14
    /* 0x00 */ UInt8 Type;
    /* 0x01 */ UInt8 pad;
    /* 0x02 */ UInt16 Flags;
    /* 0x04 */ UInt32 OutputRef;
    /* 0x08 */ UInt32 InputRef;
    /* 0x0c */ SInt32 WorkVal;
    /* 0x10 */ struct SNDModelNodeHeader_t *Next;
};

typedef struct SNDModelNodeHeader_t SNDModelNodeHeader;

typedef struct { // 0x24
    /* 0x00 */ SNDModelNodeHeader Header;
    /* 0x14 */ UInt32 NameHash;
    /* 0x18 */ SInt32 MyOut;
    /* 0x1c */ SInt32 InitialValue;
    /* 0x20 */ SInt32 InstanceRecordOffset;
} SNDModelController;

typedef struct { // 0x20
    /* 0x00 */ SNDModelNodeHeader Header;
    /* 0x14 */ UInt32 NameHash;
    /* 0x18 */ UInt32 Duration;
    /* 0x1c */ SInt32 InstanceRecordOffset;
} SNDModelEvent;

typedef struct { // 0x1c
    /* 0x00 */ SNDModelNodeHeader Header;
    /* 0x14 */ UInt8 MixType;
    /* 0x18 */ SNDModelNodeHeader **Inputs;
} SNDModelMixer;

typedef struct { // 0x8
    /* 0x0 */ SInt32 Out;
    /* 0x4 */ SInt32 In;
} SNDModelCurvePoint;

typedef struct { // 0x1c
    /* 0x00 */ SNDModelNodeHeader Header;
    /* 0x14 */ UInt32 Flags;
    /* 0x18 */ SNDModelCurvePoint *Point;
} SNDModelCurve;

typedef struct { // 0x18
    /* 0x00 */ SNDModelNodeHeader Header;
    /* 0x14 */ UInt16 Target;
    /* 0x16 */ UInt16 TargetExtra;
} SNDModelParamOut;

typedef struct { // 0x30
    /* 0x00 */ SNDModelNodeHeader Header;
    /* 0x14 */ struct SFXName SoundID;
    /* 0x28 */ UInt32 Flags;
    /* 0x2c */ SNDModelParamOut *Params;
} SNDModelSoundElement;

typedef struct { // 0x20
    /* 0x00 */ UInt32 Flags;
    /* 0x04 */ SNDModelController *Controller;
    /* 0x08 */ SNDModelEvent *Event;
    /* 0x0c */ SNDModelNodeHeader *SNDModelTickNodes;
    /* 0x10 */ SNDModelNodeHeader *MiddleNodes;
    /* 0x14 */ SNDModelSoundElement *Element;
    /* 0x18 */ SInt32 InstanceRecordSize;
    /* 0x1c */ SInt32 *InstanceTableHead;
} SNDModel;

struct SFXBlock { // 0x3c
    /* 0x00 */ UInt32 DataID;
    /* 0x04 */ UInt32 Version;
    /* 0x08 */ UInt32 Flags;
    /* 0x0c */ UInt32 BlockID;
    /* 0x10 */ SInt8 BlockNum;
    /* 0x11 */ SInt8 pad1;
    /* 0x12 */ SInt16 pad2;
    /* 0x14 */ SInt16 pad3;
    /* 0x16 */ SInt16 NumSounds;
    /* 0x18 */ SInt16 NumGrains;
    /* 0x1a */ SInt16 NumVAGs;
    /* 0x1c */ SFXPtr FirstSound;
    /* 0x20 */ SFXGrainPtr FirstGrain;
    /* 0x24 */ void *VagsInSR;
    /* 0x28 */ UInt32 VagDataSize;
    /* 0x2c */ UInt32 SRAMAllocSize;
    /* 0x30 */ struct SFXBlock *NextBlock;
    /* 0x34 */ struct SFXBlockNames *BlockNames;
    /* 0x38 */ struct SFXUserData *SFXUD;
};

typedef struct SFXBlock *SFXBlockPtr;

struct SFXBlock2 { // 0x40
    /* 0x00 */ UInt32 DataID;
    /* 0x04 */ UInt32 Version;
    /* 0x08 */ UInt32 Flags;
    /* 0x0c */ UInt32 BlockID;
    /* 0x10 */ SInt8 BlockNum;
    /* 0x11 */ SInt8 pad1;
    /* 0x12 */ SInt16 pad2;
    /* 0x14 */ SInt16 pad3;
    /* 0x16 */ SInt16 NumSounds;
    /* 0x18 */ SInt16 NumGrains;
    /* 0x1a */ SInt16 NumVAGs;
    /* 0x1c */ SFX2Ptr FirstSound;
    /* 0x20 */ SFXGrain2Ptr FirstGrain;
    /* 0x24 */ void *VagsInSR;
    /* 0x28 */ UInt32 VagDataSize;
    /* 0x2c */ UInt32 SRAMAllocSize;
    /* 0x30 */ struct SFXBlock2 *NextBlock;
    /* 0x34 */ void *GrainData;
    /* 0x38 */ struct SFXBlockNames *BlockNames;
    /* 0x3c */ struct SFXUserData *SFXUD;
};

typedef struct SFXBlock2 *SFXBlock2Ptr;

typedef struct { // 0x28
    /* 0x00 */ UInt32 DataID;
    /* 0x04 */ SInt16 Version;
    /* 0x06 */ SInt8 Flags;
    /* 0x07 */ SInt8 pad1;
    /* 0x08 */ UInt32 ID;
    /* 0x0c */ void *NextMIDIBlock;
    /* 0x10 */ UInt32 BankID;
    /* 0x14 */ SoundBankPtr BankPtr;
    /* 0x18 */ SInt8 *DataStart;
    /* 0x1c */ SInt8 *MultiMIDIParent;
    /* 0x20 */ UInt32 Tempo;
    /* 0x24 */ SInt32 PPQ;
} MIDIBlockHeader;

typedef MIDIBlockHeader *MIDIBlockHeaderPtr;

typedef struct { // 0x14
    /* 0x00 */ UInt32 DataID;
    /* 0x04 */ SInt16 Version;
    /* 0x06 */ SInt8 Flags;
    /* 0x07 */ SInt8 NumMIDIBlocks;
    /* 0x08 */ UInt32 ID;
    /* 0x0c */ void *NextMIDIBlock;
    /* 0x10 */ SInt8 *BlockPtr[1];
} MultiMIDIBlockHeader;

typedef MultiMIDIBlockHeader *MultiMIDIBlockHeaderPtr;

struct VAGStreamQueEntry { // 0x14
    /* 0x00 */ UInt32 flags;
    /* 0x04 */ SInt32 loc;
    /* 0x08 */ SInt32 offset;
    /* 0x0c */ SInt8 vol;
    /* 0x0d */ SInt8 sub_group;
    /* 0x0e */ SInt16 pan;
    /* 0x10 */ struct VAGStreamQueEntry *next;
};

struct VPKFileVolPanPair { // 0x4
    /* 0x0 */ SInt8 vol;
    /* 0x1 */ SInt8 group;
    /* 0x2 */ SInt16 pan;
};

struct VPKFileHead { // 0x1c
    /* 0x00 */ UInt32 type;
    /* 0x04 */ UInt32 size;
    /* 0x08 */ UInt32 data_offset;
    /* 0x0c */ UInt32 buff_size;
    /* 0x10 */ UInt32 sample_rate;
    /* 0x14 */ UInt32 num_channels;
    /* 0x18 */ struct VPKFileVolPanPair vol_pan[1];
};

typedef struct VPKFileHead *VPKFileHeadPtr;

struct VAGBuffer { // 0x2c
    /* 0x00 */ UInt32 flags;
    /* 0x04 */ SInt8 *IOPbuff;
    /* 0x08 */ SInt8 *SPUbuff;
    /* 0x0c */ struct VAGBuffer *list;
    /* 0x10 */ struct VAGStreamHeader *owner;
    /* 0x14 */ SInt32 is_end;
    /* 0x18 */ UInt32 bytes;
    /* 0x1c */ char *shift_from;
    /* 0x20 */ char *shift_to;
    /* 0x24 */ UInt32 shift_amount;
    /* 0x28 */ SInt8 *OrigIOPbuff;
};

typedef struct VAGBuffer *VAGBufferPtr;

struct VAGStreamHeader { // 0xd4
    /* 0x00 */ UInt32 flags;
    /* 0x04 */ SInt8 orig_vol;
    /* 0x05 */ SInt8 group;
    /* 0x06 */ SInt16 orig_pan;
    /* 0x08 */ UInt32 pitch;
    /* 0x0c */ UInt32 buff_size;
    /* 0x10 */ struct VAGBuffer buff[2];
    /* 0x68 */ SInt32 PlayingBuffer;
    /* 0x6c */ UInt32 LastPosition;
    /* 0x70 */ SInt32 LastBufferChangeTick;
    /* 0x74 */ SInt32 ErrorAccumulator;
    /* 0x78 */ SInt32 start_error_accumulator;
    /* 0x7c */ SInt32 voice;
    /* 0x80 */ SInt32 file_handle;
    /* 0x84 */ UInt32 next_read_offset;
    /* 0x88 */ UInt32 file_start_sector;
    /* 0x8c */ UInt32 file_start_offset;
    /* 0x90 */ UInt32 stdio_file_offset;
    /* 0x94 */ UInt32 ee_streammon_loc;
    /* 0x98 */ UInt32 ee_BufferSize;
    /* 0x9c */ char *ee_EEStartAddress;
    /* 0xa0 */ char *ee_NextIOPReadAddress;
    /* 0xa4 */ UInt32 ee_TotalBytesConsumed;
    /* 0xa8 */ UInt32 bytes_total;
    /* 0xac */ UInt32 bytes_remaining;
    /* 0xb0 */ UInt32 samples_total;
    /* 0xb4 */ UInt32 bytes_played;
    /* 0xb8 */ SInt32 sample_rate;
    /* 0xbc */ UInt32 last_loc;
    /* 0xc0 */ UInt32 cur_pos;
    /* 0xc4 */ SInt8 master_volume;
    /* 0xc5 */ SInt8 sub_group;
    /* 0xc6 */ SInt16 master_pan;
    /* 0xc8 */ struct VAGStreamHeader *sync_list;
    /* 0xcc */ struct VAGStreamHeader *next;
    /* 0xd0 */ struct VAGStreamHandler *handler;
};

typedef struct VAGStreamHeader *VAGStreamHeaderPtr;

struct EffectChain;
struct GenericSoundHandler;
typedef SInt32 (*EffectProcPtr)(struct EffectChain *effect, struct GenericSoundHandler *owner);

struct EffectChain { // 0x10
    /* 0x0 */ UInt32 Flags;
    /* 0x4 */ SInt16 delta_counter;
    /* 0x6 */ SInt16 delta_type;
    /* 0x8 */ struct EffectChain *next;
    /* 0xc */ EffectProcPtr proc;
};

typedef struct EffectChain *EffectChainPtr;

struct BasicEffect { // 0x1c
    /* 0x00 */ struct EffectChain ec;
    /* 0x10 */ SInt32 value;
    /* 0x14 */ SInt16 delta_value;
    /* 0x16 */ SInt16 delta_time;
    /* 0x18 */ SInt16 destination;
    /* 0x1a */ SInt16 pad;
};

typedef struct BasicEffect *BasicEffectPtr;

struct ModEffectmaster { // 0x20
    /* 0x00 */ struct EffectChain ec;
    /* 0x10 */ SInt32 num_effects;
    /* 0x14 */ EffectChainPtr first;
    /* 0x18 */ SInt32 num_params;
    /* 0x1c */ SInt32 params[1];
};

struct VolumeModEffect { // 0x2c
    /* 0x00 */ struct EffectChain ec;
    /* 0x10 */ SInt32 type;
    /* 0x14 */ SInt32 rate;
    /* 0x18 */ SInt32 depth;
    /* 0x1c */ SInt32 rate_slider_rate;
    /* 0x20 */ SInt32 rate_slider_connect;
    /* 0x24 */ SInt32 depth_slider_rate;
    /* 0x28 */ SInt32 depth_slider_connect;
};

typedef void (*OwnerProcPtr)(SInt32 voice, UInt32 owner, SInt32 flag);

struct GenericSoundHandler { // 0x34
    /* 0x00 */ UInt32 OwnerID;
    /* 0x04 */ SoundPtr Sound;
    /* 0x08 */ EffectChainPtr Effects;
    /* 0x0c */ SInt16 Original_Vol;
    /* 0x0e */ SInt16 Original_Pan;
    /* 0x10 */ SInt16 Current_Vol;
    /* 0x12 */ SInt16 Current_Pan;
    /* 0x14 */ SInt16 Current_PM;
    /* 0x16 */ UInt8 flags;
    /* 0x17 */ SInt8 VolGroup;
    /* 0x18 */ struct VoiceFlags Voices;
    /* 0x20 */ struct GenericSoundHandler *prev;
    /* 0x24 */ struct GenericSoundHandler *next;
    /* 0x28 */ struct GenericSoundHandler *parent;
    /* 0x2c */ struct GenericSoundHandler *first_child;
    /* 0x30 */ struct GenericSoundHandler *siblings;
};

typedef struct GenericSoundHandler GSoundHandler;
typedef struct GenericSoundHandler *GSoundHandlerPtr;

struct BlockSoundHandler { // 0x124
    /* 0x000 */ GSoundHandler SH;
    /* 0x034 */ SInt8 Current_Note;
    /* 0x035 */ SInt8 Current_Fine;
    /* 0x036 */ SInt16 Current_PB;
    /* 0x038 */ SInt16 LFO_Vol;
    /* 0x03a */ SInt16 App_Vol;
    /* 0x03c */ SInt16 LFO_Pan;
    /* 0x03e */ SInt16 App_Pan;
    /* 0x040 */ SInt16 LFO_PB;
    /* 0x042 */ SInt16 App_PB;
    /* 0x044 */ SInt16 LFO_PM;
    /* 0x046 */ SInt16 App_PM;
    /* 0x048 */ SInt16 CountDown;
    /* 0x04a */ SInt16 NextGrain;
    /* 0x04c */ SInt8 Registers[4];
    /* 0x050 */ SInt8 sk_doing_skipping_play;
    /* 0x051 */ SInt8 pad;
    /* 0x052 */ SInt16 sk_grains_to_play;
    /* 0x054 */ SInt16 sk_grains_to_skip;
    /* 0x056 */ UInt16 pad2;
    /* 0x058 */ SFXBlock2Ptr block;
    /* 0x05c */ struct LFOTracker lfo[4];
    /* 0x11c */ UInt32 start_tick;
    /* 0x120 */ SFX2 *orig_sound;
};

typedef struct BlockSoundHandler *BlockSoundHandlerPtr;

struct VAGStreamHandler { // 0x58
    /* 0x00 */ GSoundHandler SH;
    /* 0x34 */ SInt32 num_streams;
    /* 0x38 */ SInt32 doubling_voice;
    /* 0x3c */ SInt8 group_vols[16];
    /* 0x4c */ struct VAGStreamHeader *qued_stream;
    /* 0x50 */ struct VAGStreamQueEntry *que_list;
    /* 0x54 */ bool pre_buffer_queue;
};

typedef struct VAGStreamHandler *VAGStreamHandlerPtr;

struct SoundHandler { // 0x40
    /* 0x00 */ GSoundHandler SH;
    /* 0x34 */ SInt8 Current_Note;
    /* 0x35 */ SInt8 Current_Fine;
    /* 0x36 */ SInt16 Current_PB;
    /* 0x38 */ UInt32 StartTick;
    /* 0x3c */ UInt32 Duration;
};

typedef struct SoundHandler VAGSoundHandler;
typedef struct SoundHandler *VAGSoundHandlerPtr;

struct MIDIHandler { // 0xc0
    /* 0x00 */ GSoundHandler SH;
    /* 0x34 */ SoundBankPtr BankPtr;
    /* 0x38 */ SInt8 *DataStart;
    /* 0x3c */ SInt8 *PlayPos;
    /* 0x40 */ UInt32 TickCountdown;
    /* 0x44 */ SInt32 TickDelta;
    /* 0x48 */ UInt32 Tempo;
    /* 0x4c */ SInt32 PPT;
    /* 0x50 */ SInt32 TickError;
    /* 0x54 */ SInt16 PPQ;
    /* 0x56 */ SInt16 MuteState;
    /* 0x58 */ SInt8 Prog[16];
    /* 0x68 */ SInt8 Vol[16];
    /* 0x78 */ SInt16 Pan[16];
    /* 0x98 */ SInt16 PitchBend[16];
    /* 0xb8 */ SInt16 DamperState;
    /* 0xba */ SInt8 Flags;
    /* 0xbb */ UInt8 RunningStatus;
    /* 0xbc */ SInt8 Repeats;
    /* 0xbd */ SInt8 pad1;
    /* 0xbe */ SInt16 pad2;
};

typedef struct MIDIHandler *MIDIHandlerPtr;

typedef struct { // 0x34
    /* 0x00 */ SInt8 num_channels;
    /* 0x01 */ SInt8 basis;
    /* 0x02 */ SInt8 pad[2];
    /* 0x04 */ SInt8 channel[16];
    /* 0x14 */ SInt8 excite_min[16];
    /* 0x24 */ SInt8 excite_max[16];
} GroupDescription;

struct AMEHandler { // 0x3c4
    /* 0x000 */ GSoundHandler SH;
    /* 0x034 */ GroupDescription group[16];
    /* 0x374 */ SInt8 MIDIRegister[16];
    /* 0x384 */ UInt8 *MIDIMacro[16];
};

typedef struct AMEHandler *AMEHandlerPtr;

struct AMEStreamHeader { // 0xe8
    /* 0x00 */ MIDIBlockHeader MB;
    /* 0x28 */ struct MIDIHandler MH;
};

typedef struct AMEStreamHeader *AMEStreamHeaderPtr;

struct basic_data { // 0x4
    /* 0x0 */ UInt32 pad1;
};

struct midi_data { // 0xc
    /* 0x0 */ SInt8 MidiChannel;
    /* 0x1 */ UInt8 KeyOnVelocity;
    /* 0x2 */ SInt16 pad2;
    /* 0x4 */ SInt32 ShouldBeOff;
    /* 0x8 */ ProgPtr KeyOnProg;
};

struct block_data { // 0x4
    /* 0x0 */ SInt8 g_vol;
    /* 0x1 */ SInt8 pad;
    /* 0x2 */ SInt16 g_pan;
};

union ownerdata_tag { // 0xc
    /* 0x0 */ struct basic_data BasicData;
    /* 0x0 */ struct midi_data MIDIData;
    /* 0x0 */ struct block_data BlockData;
};

struct VoiceAttributes { // 0x38
    /* 0x00 */ struct VoiceAttributes *playlist;
    /* 0x04 */ SInt32 voice;
    /* 0x08 */ UInt32 Status;
    /* 0x0c */ GSoundHandlerPtr Owner;
    /* 0x10 */ OwnerProcPtr OwnerProc;
    /* 0x14 */ TonePtr Tone;
    /* 0x18 */ SInt8 StartNote;
    /* 0x19 */ SInt8 StartFine;
    /* 0x1a */ UInt8 Priority;
    /* 0x1b */ SInt8 VolGroup;
    /* 0x1c */ UInt32 StartTick;
    /* 0x20 */ SpuVolume Volume;
    /* 0x24 */ SInt16 Current_PB;
    /* 0x26 */ SInt16 Current_PM;
    /* 0x28 */ UInt32 Flags;
    /* 0x2c */ union ownerdata_tag OwnerData;
};

typedef struct VoiceAttributes *VoiceAttributesPtr;

struct LocAndSize { // 0x8
    /* 0x0 */ UInt32 offset;
    /* 0x4 */ UInt32 size;
};

struct FileAttributes { // 0x10
    /* 0x0 */ UInt32 type;
    /* 0x4 */ UInt32 num_chunks;
    /* 0x8 */ struct LocAndSize where[1];
};

typedef struct FileAttributes *FileAttributesPtr;

struct DuckerDef { // 0x18
    /* 0x00 */ SInt32 source_group;
    /* 0x04 */ UInt32 target_groups;
    /* 0x08 */ SInt32 full_duck_mult;
    /* 0x0c */ SInt32 attack_time;
    /* 0x10 */ SInt32 release_time;
    /* 0x14 */ SInt32 current_duck_mult;
};

typedef struct DuckerDef *DuckerDefPtr;

union bank_tag { // 0x8
    /* 0x0 */ char name[8];
    /* 0x0 */ SoundBankPtr ptr;
};

union snd_tag { // 0x10
    /* 0x0 */ char name[16];
    /* 0x0 */ SInt16 index;
};

struct SndPlayParams { // 0x30
    /* 0x00 */ union bank_tag bank_spec;
    /* 0x08 */ SInt32 vol;
    /* 0x0c */ SInt32 pan;
    /* 0x10 */ union snd_tag snd_spec;
    /* 0x20 */ SInt16 pitch_mod;
    /* 0x22 */ SInt16 pitch_bend;
    /* 0x24 */ SInt8 reg[4];
    /* 0x28 */ UInt32 mask;
    /* 0x2c */ UInt32 reserved;
};

typedef struct SndPlayParams *SndPlayParamsPtr;

struct EEVagStreamMonitor { // 0x40
    /* 0x00 */ UInt32 BufferSize;
    /* 0x04 */ char *EEStartAddress;
    /* 0x08 */ char *NextIOPReadAddress;
    /* 0x0c */ UInt32 TotalBytesConsumed;
    /* 0x10 */ char pad[48];
};

struct IOPVagStreamMonitor { // 0x40
    /* 0x00 */ unsigned int BufferSize;
    /* 0x04 */ char *IOPStartAddress;
    /* 0x08 */ char *NextIOPReadAddress;
    /* 0x0c */ int TotalBytesConsumed;
    /* 0x10 */ char pad[48];
};

typedef SInt32 (*Extern989Proc)(SInt32 arg1, SInt32 arg2, SInt32 arg3, SInt32 arg4, SInt32 arg5);

struct Extern989Handler { // 0x14
    /* 0x00 */ UInt32 proc_id;
    /* 0x04 */ SInt32 num_funcs;
    /* 0x08 */ UInt32 reserved1;
    /* 0x0c */ UInt32 reserved2;
    /* 0x10 */ Extern989Proc procs[1];
};

typedef struct Extern989Handler *Extern989HandlerPtr;

typedef struct { // 0x4
    /* 0x0 */ SInt16 left;
    /* 0x2 */ SInt16 right;
} VolPair;

typedef void *(*ExternSndIOPAlloc)(int size, int use, int *act_size);
typedef void (*ExternSndIOPFree)(void *mem);

struct GetSoundUserDataCommandStruct { // 0x24
    /* 0x00 */ u_int bank;
    /* 0x04 */ int snd_index;
    /* 0x08 */ u_int bank_name[2];
    /* 0x10 */ u_int snd_name[4];
    /* 0x20 */ int *destination;
};

typedef void (*ExternBankMonitor)(SoundBankPtr bank, SInt32 unk);
typedef void (*ExternVolMonitor)(SInt32 which, SInt32 vol);

struct Extern989MonitorInfo { // 0x8
    /* 0x0 */ ExternBankMonitor bnk_load;
    /* 0x4 */ ExternVolMonitor vol_changed;
};

struct PVSBN_struct { // 0xa0
    /* 0x00 */ char name1[64];
    /* 0x40 */ char name2[64];
    /* 0x80 */ UInt32 offset1;
    /* 0x84 */ UInt32 offset2;
    /* 0x88 */ SInt32 vol;
    /* 0x8c */ SInt32 pan;
    /* 0x90 */ SInt32 vol_group;
    /* 0x94 */ UInt32 queue;
    /* 0x98 */ SInt32 sub_group;
    /* 0x9c */ UInt32 flags;
};

typedef struct PVSBN_struct *PVSBN_structPtr;
typedef void (*SndErrorDisplayFunc)(SInt32 num, UInt32 a1, UInt32 a2, UInt32 a3, UInt32 a4);
// typedef int (*sceSdTransIntrHandler)(/* parameters unknown */);
// typedef int (*sceSdSpu2IntrHandler)(/* parameters unknown */);

// typedef struct { // 0x8
//     /* 0x0 */ short unsigned int func;
//     /* 0x2 */ short unsigned int entry;
//     /* 0x4 */ unsigned int value;
// } sceSdBatch;
//
// typedef struct { // 0x14
//     /* 0x00 */ int core;
//     /* 0x04 */ int mode;
//     /* 0x08 */ short int depth_L;
//     /* 0x0a */ short int depth_R;
//     /* 0x0c */ int delay;
//     /* 0x10 */ int feedback;
// } sceSdEffectAttr;

struct SndMessageData { // 0x20
    /* 0x00 */ int data[7];
    /* 0x1c */ int command;
};

typedef struct SndMessageData *SndMessageDataPtr;

struct SndCommandEntry { // 0x4
    /* 0x0 */ short int command;
    /* 0x2 */ short int size;
};

typedef struct SndCommandEntry *SndCommandEntryPtr;

struct SndCommandBuffer { // 0x1000
    /* 0x000 */ int num_commands;
    /* 0x004 */ char buffer[4092];
};

typedef struct SndCommandBuffer *SndCommandBufferPtr;
typedef void (*SndCompleteProc)(/* parameters unknown */);

struct SndCommandReturnDef { // 0x8
    /* 0x0 */ SndCompleteProc done;
    /* 0x4 */ u_long u_data;
};

typedef struct SndCommandReturnDef *SndCommandReturnDefPtr;

struct VAGStreamCommand { // 0x110
    /* 0x000 */ char name[256];
    /* 0x100 */ int stereo;
    /* 0x104 */ int vol;
    /* 0x108 */ int pan;
    /* 0x10c */ int vol_group;
};

struct SndSystemStatus { // 0x40
    /* 0x00 */ int cd_busy;
    /* 0x04 */ int pad_0;
    /* 0x08 */ int pad_1;
    /* 0x0c */ int pad_2;
    /* 0x10 */ int cd_error;
    /* 0x14 */ int pad_3;
    /* 0x18 */ int pad_4;
    /* 0x1c */ int pad_5;
    /* 0x20 */ char pad_big[32];
};

typedef void (*SpuTransferCallbackProc)(/* parameters unknown */);
typedef void (*CommandHandler)(SInt8 *msg_data);
typedef SInt32 (*LFOFunction)(struct LFOTracker *tracker, int step);
typedef SInt32 (*GrainHandler)(BlockSoundHandlerPtr handler, SFX2Ptr sfx, SFXGrain2Ptr grain);

struct timercommon { // 0x8
    /* 0x0 */ int *thid;
    /* 0x4 */ int compare;
};

struct sSRAMNode_tag { // 0x18
    /* 0x00 */ UInt32 loc;
    /* 0x04 */ UInt32 size;
    /* 0x08 */ UInt32 in_use;
    /* 0x0c */ struct sSRAMNode_tag *root;
    /* 0x10 */ struct sSRAMNode_tag *smaller;
    /* 0x14 */ struct sSRAMNode_tag *bigger;
};

typedef struct sSRAMNode_tag sSRAMNode;
typedef struct sSRAMNode_tag *sSRAMNodePtr;

typedef struct { // 0x4
    /* 0x0 */ SInt16 min;
    /* 0x2 */ SInt16 max;
} StreamChannelRange;

typedef struct { // 0x4
    /* 0x0 */ SInt16 min;
    /* 0x2 */ SInt16 max;
} VoiceRange;
#endif
