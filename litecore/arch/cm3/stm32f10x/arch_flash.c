#if INTFLASH_ENABLE


//Private Defines
#define STM32_INTF_LOCK_ENABLE		0



//Private Macros
#if STM32_INTF_LOCK_ENABLE
#define stm32_intf_Lock()			os_thd_Lock()
#define stm32_intf_Unlock()			os_thd_Unlock()
#else
#define stm32_intf_Lock()
#define stm32_intf_Unlock()
#endif



void arch_IntfInit()
{

	FLASH_Unlock();
	FLASH_ClearFlag(FLASH_FLAG_EOP | FLASH_FLAG_OPTERR | FLASH_FLAG_WRPRTERR | FLASH_FLAG_PGERR | FLASH_FLAG_BSY);
}

sys_res arch_IntfErase(adr_t nAdr)
{
	FLASH_Status res = FLASH_COMPLETE;
	adr_t nCur, nEndAdr;

	stm32_intf_Lock();
	nCur = nAdr;
	nEndAdr = nAdr + INTFLASH_BLK_SIZE;
	for (; nCur < nEndAdr; nCur += 4) {
		if (*(volatile uint32_t *)nCur != 0xFFFFFFFF)
			break;
	}
	if (nCur < nEndAdr)
		res = FLASH_ErasePage(nAdr);
	stm32_intf_Unlock();
	if (res == FLASH_COMPLETE)
		return SYS_R_OK;
	return SYS_R_TMO;
}

sys_res arch_IntfProgram(adr_t adr, const void *pData, uint_t nLen)
{
	adr_t nEndAdr;
	uint_t nData;
	__packed uint16_t *p = (__packed uint16_t *)pData;
	
	stm32_intf_Lock();
	nEndAdr = adr + nLen;
	for (; adr < nEndAdr; adr += 2) {
		nData = *p++;
		if (*(volatile uint16_t *)adr != nData)
			if (FLASH_ProgramHalfWord(adr, nData) != FLASH_COMPLETE)
				break;
	}
	stm32_intf_Unlock();
	if (adr < nEndAdr)
		return SYS_R_TMO;
	return SYS_R_OK;
}


#endif

