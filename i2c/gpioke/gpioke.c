// https://lab.whitequark.org/notes/2017-11-08/accessing-intel-ich-pch-gpios/
// you need to set a GOPI to 1 in order to access the SPD eeprom
// for T61(p)/X61(t)/T400/T500/W500/X200(t), the IO space address is 11B9, Bit[2] from 0 to 1,  0xFB -> 0xFF
// for T60(p)/X60(t) ： 1680, the IO space address is 1680, Bit[3] from 0 to 1

/*
 * See:
 * - http://lxr.free-electrons.com/source/drivers/mfd/lpc_ich.c
 * - http://lxr.free-electrons.com/source/drivers/gpio/gpio-ich.c
 * - Intel document 252516-001 (ICH5)
 * - Intel document 330550-002 (9 Series PCH)
 * - Intel documents 332690-004 and 332691-002EN (100 Series PCH)
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <pci/pci.h>
#include <sys/io.h>
#include <sys/mman.h>
#include <sys/errno.h>

/*
 * GPIO register offsets in GPIO I/O space.
 * Each chunk of 32 GPIOs is manipulated via its own USE_SELx, IO_SELx, and
 * LVLx registers.  Logic in the read/write functions takes a register and
 * an absolute bit number and determines the proper register offset and bit
 * number in that register.  For example, to read the value of GPIO bit 50
 * the code would access offset ichx_regs[2(=GPIO_LVL)][1(=50/32)],
 * bit 18 (50%32).
 */
enum GPIO_REG {
  GPIO_USE_SEL = 0,
  GPIO_IO_SEL,
  GPIO_LVL,
};

static const uint8_t ichx_regs[4][3] = {
  {0x00, 0x30, 0x40},     /* USE_SEL[1-3] offsets */
  {0x04, 0x34, 0x44},     /* IO_SEL[1-3] offsets */
  {0x0c, 0x38, 0x48},     /* LVL[1-3] offsets */
};

/*
 * Generic PCI configuration space registers.
 */
#define REG_VENDOR          0x00
#define REG_DEVICE          0x04

/*
 * D31:F0 configuration space registers.
 */
#define REG_ICH0_GPIOBASE   0x58
#define REG_ICH0_GC         0x5c

#define REG_ICH6_GPIOBASE   0x48
#define REG_ICH6_GC         0x4c

#define REG_ICHx_GC_EN      0x10
#define REG_ICHx_GC_GLE     0x01

/*
 * D31:F1 configuration space registers.
 */
#define REG_P2SB_BAR        0x10
#define REG_P2SB_BARH       0x14
#define REG_P2SB_CTRL       0xe0

#define REG_P2SB_CTRL_HIDE  0x0100


/*
 * Helper functions.
 */

#define MSG(...) do { \
    fprintf(stderr, "[*] " __VA_ARGS__); fprintf(stderr, "\n"); \
  } while(0)
#define ERR(...) do { \
    fprintf(stderr, "[-] " __VA_ARGS__); fprintf(stderr, "\n"); \
    return 1; \
  } while(0)
#define DIE(...) do { *fatal = 1; ERR(__VA_ARGS__) } while(0)

struct pci_dev *pci_find_dev(struct pci_access *pci, uint8_t bus, uint8_t dev, uint8_t func) {
  for(struct pci_dev *it = pci->devices; it; it = it->next) {
    if(it->bus == bus && it->dev == dev && it->func == func) return it;
  }
  return NULL;
}

/*
 * Finally, our main logic!
 */

int try_ich(struct pci_access *pci, uint16_t reg_gpiobase, uint16_t reg_gc, const char *desc, int *fatal)
{
  MSG("Checking for a %s system", desc);

  struct pci_dev *d31f0 = pci_find_dev(pci, 0, 31, 0);
  uint32_t gpiobase = pci_read_long(d31f0, reg_gpiobase);
  uint8_t gc = pci_read_byte(d31f0, reg_gc);
  MSG("GPIOBASE=%08x, GC=%02x", gpiobase, gc);

  if(gpiobase == 0xffffffff)
  {
    *fatal = 1;
    ERR("Cannot read GPIOBASE, are you running me as root?");
  } else if(gpiobase == 0) {
    ERR("GPIOBASE not implemented at %04x", reg_gpiobase);
  } else if(!(gpiobase & 1)) {
    *fatal = 1;
    ERR("GPIOBASE is not an I/O BAR");
  }

  if(!(gpiobase & 0xfffc)) {
    const uint32_t DEFAULT_GPIOBASE = 0x0480;

    MSG("GPIOBASE is not configured, setting to %08x and hoping this works", DEFAULT_GPIOBASE);
    pci_write_long(d31f0, reg_gpiobase, DEFAULT_GPIOBASE);
    gpiobase = pci_read_long(d31f0, reg_gpiobase);
    if((gpiobase & 0xfffc) != DEFAULT_GPIOBASE) {
      ERR("Cannot set GPIOBASE");
    }
  }

  MSG("GPIO decoding is %s", (gc & REG_ICHx_GC_EN) ? "enabled" : "disabled");
  MSG("GPIO lockdown is %s", (gc & REG_ICHx_GC_GLE) ? "enabled" : "disabled");

  if(!(gc & REG_ICHx_GC_EN)) {
    MSG("Enabling GPIO decoding");
    pci_write_byte(d31f0, reg_gc, gc | REG_ICHx_GC_EN);
    gc = pci_read_byte(d31f0, reg_gc);
    if(!(gc & REG_ICHx_GC_EN)) {
      ERR("Cannot enable GPIO decoding");
    }
  }

  gpiobase &= 0xfffc;
  if(ioperm(gpiobase, 128, 1) == -1) {
    ERR("Cannot access I/O ports %04x:%04x", gpiobase, gpiobase + 128);
  }

  for(int n = 1; n < 3; n++) {
    MSG("USE_SEL%d=%08x", n, inl(gpiobase + ichx_regs[GPIO_USE_SEL][n]));
    MSG("IO_SEL%d=%08x", n, inl(gpiobase + ichx_regs[GPIO_IO_SEL][n]));
    MSG("LVL%d=%08x", n, inl(gpiobase + ichx_regs[GPIO_LVL][n]));
  }
  
  
  printf("\n-----------------------------\n");
  
  uint8_t io_byte = inb(0x11B9);
  printf("\n>> original 0x11B9 : %2X", io_byte);
  
  printf("\n>> setting  0x11B9 to 0xFF");
  outb(0xFF, 0x11B9);
  
  io_byte = inb(0x11B9);
  printf("\n>> current  0x11B9 : %2X\n", io_byte);
  
  printf("\n-----------------------------\n");

  return 0;
}

int create_pci(int method, struct pci_access **pci_out)  {
  struct pci_access *pci = pci_alloc();
  pci->method = method;
  pci_init(pci);
  pci_scan_bus(pci);

  struct pci_dev *d31f0 = pci_find_dev(pci, 0, 31, 0);
  if(!d31f0) {
    ERR("Cannot find D31:F0");
  }

  pci_fill_info(d31f0, PCI_FILL_IDENT | PCI_FILL_BASES);
  if(d31f0->vendor_id != 0x8086) {
    ERR("Vendor of D31:F0 is not Intel");
  }

  *pci_out = pci;
  return 0;
}

int main()
{
  struct pci_access *pci;
  
  if(create_pci(PCI_ACCESS_AUTO, &pci)) {
    MSG("Is this an Intel platform?");
    return 1;
  }
  
  int fatal = 0;
  if(try_ich(pci, REG_ICH6_GPIOBASE, REG_ICH6_GC, "ICH6..ICH9 or Series 5..9 PCH", &fatal) && fatal)
  {
    return 1;
  }

  printf("\n[+] Done\n");
  return 0;
}
