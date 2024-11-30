// zephyr-3.7-branch/include/zephyr/arch/arm64/arm-smccc.h

#ifndef ZEPHYR_INCLUDE_ARCH_ARM64_SMCCC_H_
#define ZEPHYR_INCLUDE_ARCH_ARM64_SMCCC_H_

/**
 * @brief Result from SMC/HVC call
 *
 * This structure holds the result values from registers 0 to 7
 * after making an SMC or HVC call.
 */
struct arm_smccc_res {
	unsigned long a0; /**< Result value from register 0 */
	unsigned long a1; /**< Result value from register 1 */
	unsigned long a2; /**< Result value from register 2 */
	unsigned long a3; /**< Result value from register 3 */
	unsigned long a4; /**< Result value from register 4 */
	unsigned long a5; /**< Result value from register 5 */
	unsigned long a6; /**< Result value from register 6 */
	unsigned long a7; /**< Result value from register 7 */
};

typedef struct arm_smccc_res arm_smccc_res_t;

/**
 * @brief Enumeration for SMCCC conduit types
 *
 * This enumeration defines the types of conduits used for SMCCC calls.
 */
enum arm_smccc_conduit {
	SMCCC_CONDUIT_NONE, /**< No conduit */
	SMCCC_CONDUIT_SMC,  /**< SMC conduit */
	SMCCC_CONDUIT_HVC,  /**< HVC conduit */
};

/**
 * @brief Make HVC calls
 *
 * This function makes an HVC call with the specified function identifier
 * and parameters, and stores the results in the provided structure.
 *
 * @param a0 Function identifier
 * @param a1 Parameter register 1
 * @param a2 Parameter register 2
 * @param a3 Parameter register 3
 * @param a4 Parameter register 4
 * @param a5 Parameter register 5
 * @param a6 Parameter register 6
 * @param a7 Parameter register 7
 * @param res Pointer to the structure to store the results
 */
void arm_smccc_hvc(unsigned long a0, unsigned long a1,
		   unsigned long a2, unsigned long a3,
		   unsigned long a4, unsigned long a5,
		   unsigned long a6, unsigned long a7,
		   struct arm_smccc_res *res);

/**
 * @brief Make SMC calls
 *
 * This function makes an SMC call with the specified function identifier
 * and parameters, and stores the results in the provided structure.
 *
 * @param a0 Function identifier
 * @param a1 Parameter register 1
 * @param a2 Parameter register 2
 * @param a3 Parameter register 3
 * @param a4 Parameter register 4
 * @param a5 Parameter register 5
 * @param a6 Parameter register 6
 * @param a7 Parameter register 7
 * @param res Pointer to the structure to store the results
 */
void arm_smccc_smc(unsigned long a0, unsigned long a1,
		   unsigned long a2, unsigned long a3,
		   unsigned long a4, unsigned long a5,
		   unsigned long a6, unsigned long a7,
		   struct arm_smccc_res *res);

#endif /* ZEPHYR_INCLUDE_ARCH_ARM64_SMCCC_H_ */
//GST