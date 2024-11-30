//arch/arc/common/include/timing.c

#include <zephyr/kernel.h>
#include <zephyr/sys_clock.h>
#include <zephyr/timing/timing.h>

/**
 * @brief Initialize the timing architecture
 *
 * This function initializes the timing architecture. It is called during
 * system initialization.
 */
void arch_timing_init(void)
{
}

/**
 * @brief Start the timing architecture
 *
 * This function starts the timing architecture.
 */
void arch_timing_start(void)
{
}

/**
 * @brief Stop the timing architecture
 *
 * This function stops the timing architecture.
 */
void arch_timing_stop(void)
{
}

/**
 * @brief Get the current timing counter value
 *
 * This function retrieves the current value of the timing counter.
 *
 * @return The current timing counter value
 */
timing_t arch_timing_counter_get(void)
{
	return k_cycle_get_32();
}

/**
 * @brief Get the number of cycles between two timing points
 *
 * This function calculates the number of cycles between two timing points.
 *
 * @param start Pointer to the start timing point
 * @param end Pointer to the end timing point
 * @return The number of cycles between the start and end timing points
 */
uint64_t arch_timing_cycles_get(volatile timing_t *const start,
				volatile timing_t *const end)
{
	return (*end - *start);
}

/**
 * @brief Get the timing frequency in cycles per second
 *
 * This function retrieves the timing frequency in cycles per second.
 *
 * @return The timing frequency in cycles per second
 */
uint64_t arch_timing_freq_get(void)
{
	return sys_clock_hw_cycles_per_sec();
}

/**
 * @brief Convert cycles to nanoseconds
 *
 * This function converts the specified number of cycles to nanoseconds.
 *
 * @param cycles The number of cycles to convert
 * @return The equivalent number of nanoseconds
 */
uint64_t arch_timing_cycles_to_ns(uint64_t cycles)
{
	return k_cyc_to_ns_floor64(cycles);
}

/**
 * @brief Convert cycles to nanoseconds and average over a count
 *
 * This function converts the specified number of cycles to nanoseconds and
 * averages the result over the specified count.
 *
 * @param cycles The number of cycles to convert
 * @param count The count to average over
 * @return The average number of nanoseconds
 */
uint64_t arch_timing_cycles_to_ns_avg(uint64_t cycles, uint32_t count)
{
	return arch_timing_cycles_to_ns(cycles) / count;
}

/**
 * @brief Get the timing frequency in megahertz
 *
 * This function retrieves the timing frequency in megahertz.
 *
 * @return The timing frequency in megahertz
 */
uint32_t arch_timing_freq_get_mhz(void)
{
	return (uint32_t)(arch_timing_freq_get() / 1000000U);
}
//GST