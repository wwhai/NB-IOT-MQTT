#ifndef __DMA_H
#define	__DMA_H	   

#include "sys.h"
#include "stm32f10x_dma.h"

void MYDMA_Config(DMA_Channel_TypeDef *DMA_CHx, u32 cpar, u32 cmar, u16 cndtr);

#endif
