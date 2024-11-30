// Filename: mp.c 
 
#include <common.h> 
#include <asm/io.h> 
#include <asm/system.h> 
#include <asm/arch/mp.h> 
#include <asm/arch/soc.h> 
#include "cpu.h" 
#include <asm/arch-fsl-layerscape/soc.h> 
 
DECLARE_GLOBAL_DATA_PTR; 
 
void *get_spin_tbl_addr(void) 
{ 
 return &__spin_table; 
} 
 
phys_addr_t determine_mp_bootpg(void) 
{ 
 return (phys_addr_t)&secondary_boot_code; 
} 
 
void update_os_arch_secondary_cores(uint8_t os_arch) 
{ 
 uint64_t *table = get_spin_tbl_addr(); 
 int i; 
 
 for (i = 1; i < CONFIG_MAX_CPUS; i++) { 
 if (os_arch == IH_ARCH_DEFAULT) 
 table[i * WORDS_PER_SPIN_TABLE_ENTRY + SPIN_TABLE_ELEM_ARCH_COMP_IDX] = OS_ARCH_SAME; 
 else 
 table[i * WORDS_PER_SPIN_TABLE_ENTRY + SPIN_TABLE_ELEM_ARCH_COMP_IDX] = OS_ARCH_DIFF; 
 } 
} 
 
#ifdef CONFIG_FSL_LSCH3 
void wake_secondary_core_n(int cluster, int core, int cluster_cores) 
{ 
 struct ccsr_gur __iomem *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR); 
 struct ccsr_reset __iomem *rst = (void *)(CONFIG_SYS_FSL_RST_ADDR); 
 uint32_t mpidr = 0; 
 
 mpidr = ((cluster << 8) | core); 
 gur_out32(&gur->scratchrw[6], mpidr); 
 asm volatile("dsb st" : : : "memory"); 
 rst->brrl |= 1 << ((cluster * cluster_cores) + core); 
 asm volatile("dsb st" : : : "memory"); 
 
 while (gur_in32(&gur->scratchrw[6]) != 0) 
 ; 
} 
#endif 
 
int fsl_layerscape_wake_seconday_cores(void) 
{ 
 struct ccsr_gur __iomem *gur = (void *)(CONFIG_SYS_FSL_GUTS_ADDR); 
#ifdef CONFIG_FSL_LSCH3 
 struct ccsr_reset __iomem *rst = (void *)(CONFIG_SYS_FSL_RST_ADDR); 
 uint32_t svr, ver, cluster, type; 
 int j = 0, cluster_cores = 0; 
#elif defined(CONFIG_FSL_LSCH2) 
 struct ccsr_scfg __iomem *scfg = (void *)(CONFIG_SYS_FSL_SCFG_ADDR); 
#endif 
 uint32_t cores, cpu_up_mask = 1; 
 int i, timeout = 10; 
 uint64_t *table = get_spin_tbl_addr(); 
 
#ifdef COUNTER_FREQUENCY_REAL 
 __real_cntfrq = COUNTER_FREQUENCY_REAL; 
 flush_dcache_range((unsigned long)&__real_cntfrq, (unsigned long)&__real_cntfrq + 8); 
#endif 
 
 cores = cpu_mask(); 
 memset(table, 0, CONFIG_MAX_CPUS * SPIN_TABLE_ELEM_SIZE); 
 flush_dcache_range((unsigned long)table, (unsigned long)table + (CONFIG_MAX_CPUS * SPIN_TABLE_ELEM_SIZE)); 
 
 printf("Waking secondary cores to start from %lx\n", gd->relocaddr); 
 
#ifdef CONFIG_FSL_LSCH3 
 gur_out32(&gur->bootlocptrh, (uint32_t)(gd->relocaddr >> 32)); 
 gur_out32(&gur->bootlocptrl, (uint32_t)gd->relocaddr); 
 
 svr = gur_in32(&gur->svr); 
 ver = SVR_SOC_VER(svr); 
 if (ver == SVR_LS2080A || ver == SVR_LS2085A) { 
 gur_out32(&gur->scratchrw[6], 1); 
 asm volatile("dsb st" : : : "memory"); 
 rst->brrl = cores; 
 asm volatile("dsb st" : : : "memory"); 
 } else { 
 i = 0; 
 cluster = in_le32(&gur->tp_cluster[i].lower); 
 for (j = 0; j < TP_INIT_PER_CLUSTER; j++) { 
 type = initiator_type(cluster, j); 
 if (type && TP_ITYP_TYPE(type) == TP_ITYP_TYPE_ARM) 
 cluster_cores++; 
 } 
 
 do { 
 cluster = in_le32(&gur->tp_cluster[i].lower); 
 for (j = 0; j < TP_INIT_PER_CLUSTER; j++) { 
 type = initiator_type(cluster, j); 
 if (type && TP_ITYP_TYPE(type) == TP_ITYP_TYPE_ARM) 
 wake_secondary_core_n(i, j, cluster_cores); 
 } 
 i++; 
 } while ((cluster & TP_CLUSTER_EOC) != TP_CLUSTER_EOC); 
 } 
#elif defined(CONFIG_FSL_LSCH2) 
 scfg_out32(&scfg->scratchrw[0], (uint32_t)(gd->relocaddr >> 32)); 
 scfg_out32(&scfg->scratchrw[1], (uint32_t)gd->relocaddr); 
 asm volatile("dsb st" : : : "memory"); 
 gur_out32(&gur->brrl, cores); 
 asm volatile("dsb st" : : : "memory"); 
 
 scfg_out32(&scfg->corebcr, cores); 
#endif 
 
 asm volatile("sev"); 
 
 while (timeout--) { 
 flush_dcache_range((unsigned long)table, (unsigned long)table + CONFIG_MAX_CPUS * 64); 
 for (i = 1; i < CONFIG_MAX_CPUS; i++) { 
 if (table[i * WORDS_PER_SPIN_TABLE_ENTRY + SPIN_TABLE_ELEM_STATUS_IDX]) 
 cpu_up_mask |= 1 << i; 
 } 
 if (hweight32(cpu_up_mask) == hweight32(cores)) 
 break; 
 udelay(10); 
 } 
 if (timeout <= 0) { 
 printf("Not all cores (0x%x) are up (0x%x)\n", cores, cpu_up_mask); 
 return 1; 
 } 
 printf("All (%d) cores are up.\n", hweight32(cores)); 
 
 return 0; 
} 
 
int is_core_valid(unsigned int core) 
{ 
 return !!((1 << core) & cpu_mask()); 
} 
 
static int is_pos_valid(unsigned int pos) 
{ 
 return !!((1 << pos) & cpu_pos_mask()); 
} 
 
int is_core_online(uint64_t cpu_id) 
{ 
 uint64_t *table; 
 int pos = id_to_core(cpu_id); 
 table = (uint64_t *)get_spin_tbl_addr() + pos * WORDS_PER_SPIN_TABLE_ENTRY; 
 return table[SPIN_TABLE_ELEM_STATUS_IDX] == 1; 
} 
 
int cpu_reset(int nr) 
{ 
 puts("Feature is not implemented.\n"); 
 return 0; 
} 
 
int cpu_disable(int nr) 
{ 
 puts("Feature is not implemented.\n"); 
 return 0; 
} 
 
static int core_to_pos(int nr) 
{ 
 uint32_t cores = cpu_pos_mask(); 
 int i, count = 0; 
 
 if (nr == 0) { 
 return 0; 
 } else if (nr >= hweight32(cores)) { 
 puts("Not a valid core number.\n"); 
 return -1; 
 } 
 
 for (i = 1; i < 32; i++) { 
 if (is_pos_valid(i)) { 
 count++; 
 if (count == nr) 
 break; 
 } 
 } 
 
 if (count != nr) 
 return -1; 
 
 return i; 
} 
 
int cpu_status(int nr) 
{ 
 uint64_t *table; 
 int pos; 
 
 if (nr == 0) { 
 table = (uint64_t *)get_spin_tbl_addr(); 
 printf("table base @ 0x%p\n", table); 
 } else { 
 pos = core_to_pos(nr); 
 if (pos < 0) 
 return -1; 
 table = (uint64_t *)get_spin_tbl_addr() + pos * WORDS_PER_SPIN_TABLE_ENTRY; 
 printf("table @ 0x%p\n", table); 
 printf(" addr - 0x%016llx\n", table[SPIN_TABLE_ELEM_ENTRY_ADDR_IDX]); 
 printf(" status - 0x%016llx\n", table[SPIN_TABLE_ELEM_STATUS_IDX]); 
 printf(" lpid - 0x%016llx\n", table[SPIN_TABLE_ELEM_LPID_IDX]); 
 } 
 
 return 0; 
} 
 
int cpu_release(int nr, int argc, char * const argv[]) 
{ 
 uint64_t boot_addr; 
 uint64_t *table = (uint64_t *)get_spin_tbl_addr(); 
 int pos; 
 
 pos = core_to_pos(nr); 
 if (pos <= 0) 
 return -1; 
 
 table += pos * WORDS_PER_SPIN_TABLE_ENTRY; 
 boot_addr = simple_strtoull(argv[0], NULL, 16); 
 table[SPIN_TABLE_ELEM_ENTRY_ADDR_IDX] = boot_addr; 
 flush_dcache_range((unsigned long)table, (unsigned long)table + SPIN_TABLE_ELEM_SIZE); 
 asm volatile("dsb st"); 
 smp_kick_all_cpus(); 
 asm volatile("sev"); 
 
 return 0; 
} 
 
// By GST @Date ok