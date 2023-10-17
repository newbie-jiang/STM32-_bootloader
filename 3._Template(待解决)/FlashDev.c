
 
#include "..\FlashOS.H"        // FlashOS Structures



struct FlashDevice const FlashDevice  =  {
    FLASH_DRV_VERS,                   	// Driver Version, do not modify!
    "STM32H750XB_W25Q64",   					// Device Name  
    EXTSPI,                            // Device Type
    0x90000000,                       	// Device Start Address
    8 * 1024 * 1024,                 	// Device Size in Bytes (8MB)
    4 * 1024,                         // Programming Page Size 
    0,                               	// Reserved, must be 0
    0xFF,                             	// Initial Content of Erased Memory
    6000,                             	// Program Page Timeout
    15000,                              // Erase Sector Timeout 
    64 * 1024, 0x000000,              	// Sector Size  64kB and Address of Sectors	
    SECTOR_END    
};

