OUTPUT_FORMAT("elf32-littleriscv", "elf32-littleriscv",
              "elf32-littleriscv")
OUTPUT_ARCH(riscv)
ENTRY(_fdm_start)

/* guoy: Stack and heap size */
_STACK_SIZE = 0x3000;
_HEAP_SIZE = 0x1000;
_RTL_CHECK = 0x1800f;
/* guoy:  Stack and heap size done */

/* Peripheral registers memory address */
MEMORY
{
   /******************* memory space *********************/
   P_MEMORY	: ORIGIN = 0x100000, LENGTH = 0x10000
   D_MEMORY	: ORIGIN = 0x10000, LENGTH = 0x10000
 
   
   /******************* Peripheral registers address  *******************/
   DEV_EMU     : ORIGIN = 0x0001100, LENGTH = 0x000300 /* device emulation registers, inside cpu subsystem */
   FLASH_REGS  : ORIGIN = 0x0001500, LENGTH = 0x0000C0     /* FLASH registers */
   CSM         : ORIGIN = 0x00015C0, LENGTH = 0x000020     /* code security module registers */
  
   ADC_MIRROR  : ORIGIN = 0x0001600, LENGTH = 0x000020     /* ADC Results register mirror */

   XINTF       : ORIGIN = 0x0001640, LENGTH = 0x000040     /* external interface registers */
   
   /* now cpu timer is put in system bus, as a peripheral */
   CPU_TIMER0  : ORIGIN = 0x0001800, LENGTH = 0x000010     /* cpu Timer0 registers */
   CPU_TIMER1  : ORIGIN = 0x0001810, LENGTH = 0x000010   /* cpu Timer0 registers */
   CPU_TIMER2  : ORIGIN = 0x0001820, LENGTH = 0x000010   /* cpu Timer0 registers */

   DMA         : ORIGIN = 0x002000, LENGTH = 0x000400     /* DMA registers */

   MCBSPA      : ORIGIN = 0x00A000, LENGTH = 0x000080     /* McBSP-A registers */
   MCBSPB      : ORIGIN = 0x00A080, LENGTH = 0x000080     /* McBSP-B registers */

   ECANA       : ORIGIN = 0x00C000, LENGTH = 0x000080     /* eCAN-A control and status registers */ 
   ECANA_LAM   : ORIGIN = 0x00C080, LENGTH = 0x000080     /* eCAN-A local acceptance masks */
   ECANA_MOTS  : ORIGIN = 0x00C100, LENGTH = 0x000080     /* eCAN-A message object time stamps */
   ECANA_MOTO  : ORIGIN = 0x00C180, LENGTH = 0x000080     /* eCAN-A object time-out registers */
   ECANA_MBOX  : ORIGIN = 0x00C200, LENGTH = 0x000200     /* eCAN-A mailboxes */

   ECANB       : ORIGIN = 0x00C400, LENGTH = 0x000080     /* eCAN-B control and status registers */ 
   ECANB_LAM   : ORIGIN = 0x00C480, LENGTH = 0x000080     /* eCAN-B local acceptance masks */
   ECANB_MOTS  : ORIGIN = 0x00C500, LENGTH = 0x000080     /* eCAN-B message object time stamps */
   ECANB_MOTO  : ORIGIN = 0x00C580, LENGTH = 0x000080     /* eCAN-B object time-out registers */
   ECANB_MBOX  : ORIGIN = 0x00C600, LENGTH = 0x000200     /* eCAN-B mailboxes */

   EPWM1       : ORIGIN = 0x00D000, LENGTH = 0x000044     /* Enhanced PWM 1 registers */
   EPWM2       : ORIGIN = 0x00D080, LENGTH = 0x000044     /* Enhanced PWM 2 registers */
   EPWM3       : ORIGIN = 0x00D100, LENGTH = 0x000044     /* Enhanced PWM 3 registers */
   EPWM4       : ORIGIN = 0x00D180, LENGTH = 0x000044     /* Enhanced PWM 4 registers */
   EPWM5       : ORIGIN = 0x00D200, LENGTH = 0x000044     /* Enhanced PWM 5 registers */
   EPWM6       : ORIGIN = 0x00D280, LENGTH = 0x000044     /* Enhanced PWM 6 registers */

   ECAP1       : ORIGIN = 0x00D400, LENGTH = 0x000040     /* Enhanced Capture 1 registers */
   ECAP2       : ORIGIN = 0x00D440, LENGTH = 0x000040     /* Enhanced Capture 2 registers */
   ECAP3       : ORIGIN = 0x00D480, LENGTH = 0x000040     /* Enhanced Capture 3 registers */
   ECAP4       : ORIGIN = 0x00D4C0, LENGTH = 0x000040     /* Enhanced Capture 4 registers */         
   ECAP5       : ORIGIN = 0x00D500, LENGTH = 0x000040     /* Enhanced Capture 5 registers */         
   ECAP6       : ORIGIN = 0x00D540, LENGTH = 0x000040     /* Enhanced Capture 6 registers */         
 
   EQEP1       : ORIGIN = 0x00D600, LENGTH = 0x000080     /* Enhanced QEP 1 registers */
   EQEP2       : ORIGIN = 0x00D680, LENGTH = 0x000080     /* Enhanced QEP 2 registers */   
   
   GPIOCTRL    : ORIGIN = 0x00DF00, LENGTH = 0x000080     /* GPIO control registers */
   GPIODAT     : ORIGIN = 0x00DF80, LENGTH = 0x000040     /* GPIO data registers */
   GPIOINT     : ORIGIN = 0x00DFC0, LENGTH = 0x000040     /* GPIO interrupt/LPM registers */
   
   SYSTEM      : ORIGIN = 0x00E020, LENGTH = 0x000040     /* System control registers */
   SPIA        : ORIGIN = 0x00E080, LENGTH = 0x000020     /* SPI-A registers */
   SCIA        : ORIGIN = 0x00E0A0, LENGTH = 0x000020     /* SCI-A registers */

   ADC         : ORIGIN = 0x00E200, LENGTH = 0x000040     /* ADC registers */

   SCIB        : ORIGIN = 0x00EEA0, LENGTH = 0x000020     /* SCI-B registers */

   SCIC        : ORIGIN = 0x00EEE0, LENGTH = 0x000020     /* SCI-C registers */
   
   I2CA        : ORIGIN = 0x00F200, LENGTH = 0x000080     /* I2C-A registers */
   
   XINTRUPT    : ORIGIN = 0x008001B4, LENGTH = 0x000016    /* external interrupt registers */
   CSM_PWL     : ORIGIN = 0x33FFF8 * 2, LENGTH = 0x000010 * 2   /* Part of FLASHA.  CSM password locations. */

   PARTID      : ORIGIN = 0x380090 * 2, LENGTH = 0x000002 * 2    /* Part ID register location */
}

/* Peripheral registers memory address section mapping*/
SECTIONS
{

/*** Peripheral Frame 0 Register Structures ***/
   DevEmuRegsFile  (NOLOAD)  : { *(DevEmuRegsFile) } > DEV_EMU 
   FlashRegsFile  (NOLOAD)   : { *(FlashRegsFile) } > FLASH_REGS
   CsmRegsFile    (NOLOAD)   : { *(CsmRegsFile) } > CSM
   AdcMirrorFile  (NOLOAD)   : { *(AdcMirrorFile) } > ADC_MIRROR
   XintfRegsFile  (NOLOAD)   : { *(XintfRegsFile) } > XINTF
  CpuTimer0RegsFile (NOLOAD) : { *(CpuTimer0RegsFile) } > CPU_TIMER0
  CpuTimer1RegsFile (NOLOAD) : { *(CpuTimer1RegsFile) } > CPU_TIMER1
  CpuTimer2RegsFile (NOLOAD) : { *(CpuTimer2RegsFile) } > CPU_TIMER2
   DmaRegsFile     (NOLOAD)  : { *(DmaRegsFile) } > DMA 

/*** Peripheral Frame 3 Register Structures ***/
   McbspaRegsFile  (NOLOAD)  : { *(McbspaRegsFile) } > MCBSPA
   McbspbRegsFile   (NOLOAD) : { *(McbspbRegsFile) } > MCBSPB

/*** Peripheral Frame 1 Register Structures ***/
   ECanaRegsFile   (NOLOAD)  : { *(ECanaRegsFile) } > ECANA
   ECanaLAMRegsFile (NOLOAD) : { *(ECanaLAMRegsFile) } > ECANA_LAM   
   ECanaMboxesFile  (NOLOAD) : { *(ECanaMboxesFile) } > ECANA_MBOX
   ECanaMOTSRegsFile (NOLOAD) : { *(ECanaMOTSRegsFile) } > ECANA_MOTS
   ECanaMOTORegsFile (NOLOAD) : { *(ECanaMOTORegsFile) } > ECANA_MOTO
   
   ECanbRegsFile  (NOLOAD)   : { *(ECanbRegsFile) } > ECANB
   ECanbLAMRegsFile (NOLOAD) : { *(ECanbLAMRegsFile) } > ECANB_LAM   
   ECanbMboxesFile  (NOLOAD) : { *(ECanbMboxesFile) } > ECANB_MBOX
   ECanbMOTSRegsFile (NOLOAD) : { *(ECanbMOTSRegsFile) } > ECANB_MOTS
   ECanbMOTORegsFile (NOLOAD) : { *(ECanbMOTORegsFile) } > ECANB_MOTO
   
   EPwm1RegsFile  (NOLOAD)   : { *(EPwm1RegsFile) } > EPWM1   
   EPwm2RegsFile   (NOLOAD)  : { *(EPwm2RegsFile) } > EPWM2   
   EPwm3RegsFile   (NOLOAD)  : { *(EPwm3RegsFile) } > EPWM3   
   EPwm4RegsFile  (NOLOAD)   : { *(EPwm4RegsFile) } > EPWM4   
   EPwm5RegsFile   (NOLOAD)  : { *(EPwm5RegsFile) } > EPWM5   
   EPwm6RegsFile  (NOLOAD)   : { *(EPwm6RegsFile) } > EPWM6
   
   ECap1RegsFile  (NOLOAD)   : { *(ECap1RegsFile) } > ECAP1   
   ECap2RegsFile  (NOLOAD)   : { *(ECap2RegsFile) } > ECAP2   
   ECap3RegsFile   (NOLOAD)  : { *(ECap3RegsFile) } > ECAP3   
   ECap4RegsFile   (NOLOAD)  : { *(ECap4RegsFile) } > ECAP4
   ECap5RegsFile  (NOLOAD)   : { *(ECap5RegsFile) } > ECAP5   
   ECap6RegsFile  (NOLOAD)   : { *(ECap6RegsFile) } > ECAP6

   EQep1RegsFile  (NOLOAD)   : { *(EQep1RegsFile) } > EQEP1   
   EQep2RegsFile  (NOLOAD)   : { *(EQep2RegsFile) } > EQEP2               

   GpioCtrlRegsFile (NOLOAD) : { *(GpioCtrlRegsFile) } > GPIOCTRL
   GpioDataRegsFile (NOLOAD)  : { *(GpioDataRegsFile) } > GPIODAT
   GpioIntRegsFile (NOLOAD)  : { *(GpioIntRegsFile) } > GPIOINT
   
/*** Peripheral Frame 2 Register Structures ***/
   SysCtrlRegsFile (NOLOAD)  : { *(SysCtrlRegsFile) } > SYSTEM
   SpiaRegsFile    (NOLOAD)  : { *(SpiaRegsFile) } > SPIA
   SciaRegsFile    (NOLOAD)  : { *(SciaRegsFile) } > SCIA
   XIntruptRegsFile (NOLOAD) : { *(XIntruptRegsFile) } > XINTRUPT
   AdcRegsFile    (NOLOAD)   : { *(AdcRegsFile) } > ADC
   ScibRegsFile   (NOLOAD)   : { *(ScibRegsFile) } > SCIB
   ScicRegsFile    (NOLOAD)  : { *(ScicRegsFile) } > SCIC
   I2caRegsFile   (NOLOAD)   : { *(I2caRegsFile) } > I2CA
              
/*** Code Security Module Register Structures ***/
  CsmPwlFile     (NOLOAD)   : { *(CsmPwlFile) } > CSM_PWL 

/*** Device Part ID Register Structures ***/
  PartIdRegsFile  (NOLOAD)  : { *(PartIdRegsFile) } > PARTID 

}

SECTIONS
{
   /*dmem start here*/
  /* guoy: put data in dmemory, not pmemory  */
   . = 0x00010000; 

  .data           :
  {
    __DATA_BEGIN__ = .;
    *(.data .data.* .gnu.linkonce.d.*)
    SORT(CONSTRUCTORS)
  }
  .data1          : { *(.data1) }
  .got            : { *(.got.plt) *(.igot.plt) *(.got) *(.igot) }
  /* We want the small data sections together, so single-instruction offsets
     can access them all, and initialized data all before uninitialized, so
     we can shorten the on-disk segment size.  */
  .sdata          :
  {
    __SDATA_BEGIN__ = .;
    *(.srodata.cst16) *(.srodata.cst8) *(.srodata.cst4) *(.srodata.cst2) *(.srodata .srodata.*)
    *(.sdata .sdata.* .gnu.linkonce.s.*)
  }
  _edata = .;
  PROVIDE (edata = .);
  . = .;
  .fini           :
  {
    KEEP (*(SORT_NONE(.fini)))
  }
  PROVIDE (__etext = .);
  PROVIDE (_etext = .);
  PROVIDE (etext = .);
  .rodata         : { *(.rodata .rodata.* .gnu.linkonce.r.*) }
  .rodata1        : { *(.rodata1) }
  .sdata2         :
  {
    *(.sdata2 .sdata2.* .gnu.linkonce.s2.*)
  }
  .sbss2          : { *(.sbss2 .sbss2.* .gnu.linkonce.sb2.*) }
  .eh_frame_hdr   : { *(.eh_frame_hdr) *(.eh_frame_entry .eh_frame_entry.*) }
  
  .gcc_except_table   : ONLY_IF_RO { *(.gcc_except_table .gcc_except_table.*) }
  .gnu_extab   : ONLY_IF_RO { *(.gnu_extab*) }
  /* These sections are generated by the Sun/Oracle C++ compiler.  */
  .exception_ranges   : ONLY_IF_RO { *(.exception_ranges*) }
  /* Adjust the address for the data segment.  We want to adjust up to
     the same address within the page on the next page up.  */
  . = DATA_SEGMENT_ALIGN (CONSTANT (MAXPAGESIZE), CONSTANT (COMMONPAGESIZE));
  /* Exception handling  */
 
  .gnu_extab      : ONLY_IF_RW { *(.gnu_extab) }
  .gcc_except_table   : ONLY_IF_RW { *(.gcc_except_table .gcc_except_table.*) }
  .exception_ranges   : ONLY_IF_RW { *(.exception_ranges*) }
  /* Thread Local Storage sections  */
  .tdata	  :
   {
     PROVIDE_HIDDEN (__tdata_start = .);
     *(.tdata .tdata.* .gnu.linkonce.td.*)
   }
  .tbss		  : { *(.tbss .tbss.* .gnu.linkonce.tb.*) *(.tcommon) }
 
  .ctors          :
  {
    /* gcc uses crtbegin.o to find the start of
       the constructors, so we make sure it is
       first.  Because this is a wildcard, it
       doesn't matter if the user does not
       actually link against crtbegin.o; the
       linker won't look for a file to match a
       wildcard.  The wildcard also means that it
       doesn't matter which directory crtbegin.o
       is in.  */
    KEEP (*crtbegin.o(.ctors))
    KEEP (*crtbegin?.o(.ctors))
    /* We don't want to include the .ctor section from
       the crtend.o file until after the sorted ctors.
       The .ctor section from the crtend file contains the
       end of ctors marker and it must be last */
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .ctors))
    KEEP (*(SORT(.ctors.*)))
    KEEP (*(.ctors))
  }
  .dtors          :
  {
    KEEP (*crtbegin.o(.dtors))
    KEEP (*crtbegin?.o(.dtors))
    KEEP (*(EXCLUDE_FILE (*crtend.o *crtend?.o ) .dtors))
    KEEP (*(SORT(.dtors.*)))
    KEEP (*(.dtors))
  }
  .jcr            : { KEEP (*(.jcr)) }
  .data.rel.ro : { *(.data.rel.ro.local* .gnu.linkonce.d.rel.ro.local.*) *(.data.rel.ro .data.rel.ro.* .gnu.linkonce.d.rel.ro.*) }
  .dynamic        : { *(.dynamic) }
  
    __global_pointer$ = MIN(__SDATA_BEGIN__ + 0x800,
		            MAX(__DATA_BEGIN__ + 0x800, __BSS_END__ - 0x800));

	/* stack and heap space alloc */
	.heap (NOLOAD) : {
	   . = ALIGN(8);
	   _heap = .;
	   _heap_start = .;
	   . += _HEAP_SIZE;
	   _heap_end = .;
	}
	
	.stack (NOLOAD) : {
	   . += _STACK_SIZE;
	   _lstack = ABSOLUTE(.);
	   _lstack_end = ABSOLUTE(MAX(_lstack, _RTL_CHECK));
           . =   ABSOLUTE(_lstack_end);
	   . = ALIGN(8);
	   _stack = .;
	   __stack = _stack;
	   __stack_top = _stack;
          /*-----added by zhangyuchao 2023-02-16 start */
          /*-----added by zhangyuchao 2023-02-16  end  */
	}

   /* pmem starts here */
    . = 0x000100000;
    .text_fdm_start : { *(.text_fdm_start) } 
  /* Read-only sections, merged into text segment: */
  # PROVIDE (__executable_start = SEGMENT_START("text-segment", 0x10000));
  # . = SEGMENT_START("text-segment", 0x10000) + SIZEOF_HEADERS;

  /* . = ALIGN(0x1000); */
  . = . + SIZEOF_HEADERS;
  .text-segment :{*(.text-segment)}
  
  .interp         : { *(.interp) }
  .note.gnu.build-id  : { *(.note.gnu.build-id) }
  .hash           : { *(.hash) }
  .gnu.hash       : { *(.gnu.hash) }
  .dynsym         : { *(.dynsym) }
  .dynstr         : { *(.dynstr) }
  .gnu.version    : { *(.gnu.version) }
  .gnu.version_d  : { *(.gnu.version_d) }
  .gnu.version_r  : { *(.gnu.version_r) }
  .rela.dyn       :
    {
      *(.rela.init)
      *(.rela.text .rela.text.* .rela.gnu.linkonce.t.*)
      *(.rela.fini)
      *(.rela.rodata .rela.rodata.* .rela.gnu.linkonce.r.*)
      *(.rela.data .rela.data.* .rela.gnu.linkonce.d.*)
      *(.rela.tdata .rela.tdata.* .rela.gnu.linkonce.td.*)
      *(.rela.tbss .rela.tbss.* .rela.gnu.linkonce.tb.*)
      *(.rela.ctors)
      *(.rela.dtors)
      *(.rela.got)
      *(.rela.sdata .rela.sdata.* .rela.gnu.linkonce.s.*)
      *(.rela.sbss .rela.sbss.* .rela.gnu.linkonce.sb.*)
      *(.rela.sdata2 .rela.sdata2.* .rela.gnu.linkonce.s2.*)
      *(.rela.sbss2 .rela.sbss2.* .rela.gnu.linkonce.sb2.*)
      *(.rela.bss .rela.bss.* .rela.gnu.linkonce.b.*)
      *(.rela.ifunc)
    }
  .rela.plt       :
    {
      *(.rela.plt)
      PROVIDE_HIDDEN (__rela_iplt_start = .);
      *(.rela.iplt)
      PROVIDE_HIDDEN (__rela_iplt_end = .);
    }
  .init           :
  {
    KEEP (*(SORT_NONE(.init)))
  }
  .plt            : { *(.plt) *(.iplt) }
  .text           :
  {
    *(.text.unlikely .text.*_unlikely .text.unlikely.*)
    *(.text.exit .text.exit.*)
    *(.text.startup .text.startup.*)
    *(.text.hot .text.hot.*)
    *(SORT(.text.sorted.*))
    *(.text .stub .text.* .gnu.linkonce.t.*)
    /* .gnu.warning sections are handled specially by elf.em.  */
    *(.gnu.warning)
  }
  __bss_start = .;
  .sbss           :
  {
    *(.dynsbss)
    *(.sbss .sbss.* .gnu.linkonce.sb.*)
    *(.scommon)
  }
  .bss            :
  {
   *(.dynbss)
   *(.bss .bss.* .gnu.linkonce.b.*)
   *(COMMON)
   /* Align here to ensure that the .bss section occupies space up to
      _end.  Align after .bss to ensure correct alignment even if the
      .bss section disappears because there are no input sections.
      FIXME: Why do we need it? When there is no .bss section, we do not
      pad the .data section.  */
   . = ALIGN(. != 0 ? 32 / 8 : 1);
  }
  . = ALIGN(32 / 8);
  . = SEGMENT_START("ldata-segment", .);
  . = ALIGN(32 / 8);
  __BSS_END__ = .;
   .preinit_array    :
  {
    PROVIDE_HIDDEN (__preinit_array_start = .);
    KEEP (*(.preinit_array))
    PROVIDE_HIDDEN (__preinit_array_end = .);
  }
   .init_array    :
  {
    PROVIDE_HIDDEN (__init_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.init_array.*) SORT_BY_INIT_PRIORITY(.ctors.*)))
    KEEP (*(.init_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .ctors))
    PROVIDE_HIDDEN (__init_array_end = .);
  }
  .fini_array    :
  {
    PROVIDE_HIDDEN (__fini_array_start = .);
    KEEP (*(SORT_BY_INIT_PRIORITY(.fini_array.*) SORT_BY_INIT_PRIORITY(.dtors.*)))
    KEEP (*(.fini_array EXCLUDE_FILE (*crtbegin.o *crtbegin?.o *crtend.o *crtend?.o ) .dtors))
    PROVIDE_HIDDEN (__fini_array_end = .);
  }
   .eh_frame       : ONLY_IF_RW { KEEP (*(.eh_frame)) *(.eh_frame.*) }
  .eh_frame       : ONLY_IF_RO { KEEP (*(.eh_frame)) *(.eh_frame.*) }
  . = DATA_SEGMENT_RELRO_END (0, .);

  _end = .; PROVIDE (end = .);
  . = DATA_SEGMENT_END (.);
  /* Stabs debugging sections.  */
  .stab          0 : { *(.stab) }
  .stabstr       0 : { *(.stabstr) }
  .stab.excl     0 : { *(.stab.excl) }
  .stab.exclstr  0 : { *(.stab.exclstr) }
  .stab.index    0 : { *(.stab.index) }
  .stab.indexstr 0 : { *(.stab.indexstr) }
  .comment       0 : { *(.comment) }
  .gnu.build.attributes : { *(.gnu.build.attributes .gnu.build.attributes.*) }
  /* DWARF debug sections.
     Symbols in the DWARF debugging sections are relative to the beginning
     of the section so we begin them at 0.  */
  /* DWARF 1.  */
  .debug          0 : { *(.debug) }
  .line           0 : { *(.line) }
  /* GNU DWARF 1 extensions.  */
  .debug_srcinfo  0 : { *(.debug_srcinfo) }
  .debug_sfnames  0 : { *(.debug_sfnames) }
  /* DWARF 1.1 and DWARF 2.  */
  .debug_aranges  0 : { *(.debug_aranges) }
  .debug_pubnames 0 : { *(.debug_pubnames) }
  /* DWARF 2.  */
  .debug_info     0 : { *(.debug_info .gnu.linkonce.wi.*) }
  .debug_abbrev   0 : { *(.debug_abbrev) }
  .debug_line     0 : { *(.debug_line .debug_line.* .debug_line_end) }
  .debug_frame    0 : { *(.debug_frame) }
  .debug_str      0 : { *(.debug_str) }
  .debug_loc      0 : { *(.debug_loc) }
  .debug_macinfo  0 : { *(.debug_macinfo) }
  /* SGI/MIPS DWARF 2 extensions.  */
  .debug_weaknames 0 : { *(.debug_weaknames) }
  .debug_funcnames 0 : { *(.debug_funcnames) }
  .debug_typenames 0 : { *(.debug_typenames) }
  .debug_varnames  0 : { *(.debug_varnames) }
  /* DWARF 3.  */
  .debug_pubtypes 0 : { *(.debug_pubtypes) }
  .debug_ranges   0 : { *(.debug_ranges) }
  /* DWARF 5.  */
  .debug_addr     0 : { *(.debug_addr) }
  .debug_line_str 0 : { *(.debug_line_str) }
  .debug_loclists 0 : { *(.debug_loclists) }
  .debug_macro    0 : { *(.debug_macro) }
  .debug_names    0 : { *(.debug_names) }
  .debug_rnglists 0 : { *(.debug_rnglists) }
  .debug_str_offsets 0 : { *(.debug_str_offsets) }
  .debug_sup      0 : { *(.debug_sup) }
  .gnu.attributes 0 : { KEEP (*(.gnu.attributes)) }
  /DISCARD/ : { *(.note.GNU-stack) *(.gnu_debuglink) *(.gnu.lto_*) }
}
