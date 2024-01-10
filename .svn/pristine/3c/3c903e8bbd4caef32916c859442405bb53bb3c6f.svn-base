/*
             LUFA Library
     Copyright (C) Dean Camera, 2017.

  dean [at] fourwalledcubicle [dot] com
           www.lufa-lib.org
*/

/*
  Copyright 2017  Dean Camera (dean [at] fourwalledcubicle [dot] com)

  Permission to use, copy, modify, distribute, and sell this
  software and its documentation for any purpose is hereby granted
  without fee, provided that the above copyright notice appear in
  all copies and that both that the copyright notice and this
  permission notice and warranty disclaimer appear in supporting
  documentation, and that the name of the author not be used in
  advertising or publicity pertaining to distribution of the
  software without specific, written prior permission.

  The author disclaims all warranties with regard to this
  software, including all implied warranties of merchantability
  and fitness.  In no event shall the author be liable for any
  special, indirect or consequential damages or any damages
  whatsoever resulting from loss of use, data or profits, whether
  in an action of contract, negligence or other tortious action,
  arising out of or in connection with the use or performance of
  this software.
*/


#ifndef _MS_CLASS_COMMON_H_
#define _MS_CLASS_COMMON_H_


/* Macros: */

#define MS_CBW_SIGNATURE          0x43425355UL 		/** Magic signature for a Command Block Wrapper used in the Mass Storage Bulk-Only transport protocol. */
#define MS_CSW_SIGNATURE          0x53425355UL 		/** Magic signature for a Command Status Wrapper used in the Mass Storage Bulk-Only transport protocol. */
#define MS_COMMAND_DIR_DATA_OUT   (0 << 7) /** Mask for a Command Block Wrapper's flags attribute to specify a command with data sent from host-to-device. */
#define MS_COMMAND_DIR_DATA_IN    (1 << 7)		/** Mask for a Command Block Wrapper's flags attribute to specify a command with data sent from device-to-host. */

/** SCSI Commands
 */
#define SCSI_CMD_INQUIRY            0x12 	/** SCSI Command Code for an INQUIRY command. */
#define SCSI_CMD_REQUEST_SENSE      0x03 	/** REQUEST SENSE command. */
#define SCSI_CMD_TEST_UNIT_READY    0x00 	/** TEST UNIT READY command. */
#define SCSI_CMD_READ_CAPACITY_10   0x25 	/** READ CAPACITY (10) command. */
#define SCSI_CMD_START_STOP_UNIT    0x1B 	/** START STOP UNIT command. */
#define SCSI_CMD_SEND_DIAGNOSTIC    0x1D  /** SEND DIAGNOSTIC command. */
#define SCSI_CMD_NO_ALLOW_REMOVAL   0x1E	 /** PREVENT ALLOW MEDIUM REMOVAL command. */
#define SCSI_CMD_WRITE_10           0x2A 	/** WRITE (10) command. */
#define SCSI_CMD_READ_10            0x28 	/** READ (10) command. */
#define SCSI_CMD_WRITE_6            0x0A 	/** WRITE (6) command. */
#define SCSI_CMD_READ_6             0x08 	/** READ (6) command. */
#define SCSI_CMD_VERIFY_10          0x2F 	/** VERIFY (10) command. */
#define SCSI_CMD_MODE_SENSE_6       0x1A		/** MODE SENSE (6) command. */
#define SCSI_CMD_MODE_SENSE_10      0x5A 	/** MODE SENSE (10) command. */


/** SCSI Sense Key Values
 */
#define SCSI_SENSE_KEY_GOOD             0x00 	/** SCSI Sense Code to indicate no error has occurred. */
#define SCSI_SENSE_KEY_RECOVERED_ERROR  0x01 	/** SCSI Sense Code to indicate that the device has recovered from an error. */
#define SCSI_SENSE_KEY_NOT_READY        0x02 	/** SCSI Sense Code to indicate that the device is not ready for a new command. */
#define SCSI_SENSE_KEY_MEDIUM_ERROR     0x03 	/** SCSI Sense Code to indicate an error whilst accessing the medium. */
#define SCSI_SENSE_KEY_HARDWARE_ERROR   0x04  /** SCSI Sense Code to indicate a hardware error has occurred. */
#define SCSI_SENSE_KEY_ILLEGAL_REQUEST  0x05  /** SCSI Sense Code to indicate that an illegal request has been issued. */
#define SCSI_SENSE_KEY_UNIT_ATTENTION   0x06  /** SCSI Sense Code to indicate that the unit requires attention from the host to indicate		 *  a reset event, medium removal or other condition. 		 */
#define SCSI_SENSE_KEY_DATA_PROTECT     0x07  /** SCSI Sense Code to indicate that a write attempt on a protected block has been made. */
#define SCSI_SENSE_KEY_BLANK_CHECK      0x08  /** SCSI Sense Code to indicate an error while trying to write to a write-once medium. */
#define SCSI_SENSE_KEY_VENDOR_SPECIFIC  0x09  /** SCSI Sense Code to indicate a vendor specific error has occurred. */
#define SCSI_SENSE_KEY_COPY_ABORTED     0x0A  /** SCSI Sense Code to indicate that an EXTENDED COPY command has aborted due to an error. */
#define SCSI_SENSE_KEY_ABORTED_COMMAND  0x0B  /** SCSI Sense Code to indicate that the device has aborted the issued command. */
#define SCSI_SENSE_KEY_VOLUME_OVERFLOW  0x0D  /** SCSI Sense Code to indicate an attempt to write past the end of a partition has been made. */
#define SCSI_SENSE_KEY_MISCOMPARE       0x0E  /** SCSI Sense Code to indicate that the source data did not match the data read from the medium. */

/** SCSI Additional Sense Codes
 */
#define SCSI_ASENSE_NO_ADDITIONAL_INFORMATION          0x00		/** SCSI Additional Sense Code to indicate no additional sense information is available. */
#define SCSI_ASENSE_LOGICAL_UNIT_NOT_READY             0x04		/** SCSI Additional Sense Code to indicate that the logical unit (LUN) addressed is not ready. */
#define SCSI_ASENSE_INVALID_FIELD_IN_CDB               0x24		/** SCSI Additional Sense Code to indicate an invalid field was encountered while processing the issued command. */
#define SCSI_ASENSE_NOT_READY_TO_READY_CHANGE          0x28		/** SCSI Additional Sense Code to indicate that a medium that was previously indicated as not ready has now		 *  become ready for use.		 */
#define SCSI_ASENSE_WRITE_PROTECTED                    0x27		/** SCSI Additional Sense Code to indicate that an attempt to write to a protected area was made. */
#define SCSI_ASENSE_FORMAT_ERROR                       0x31 		/** SCSI Additional Sense Code to indicate an error whilst formatting the device medium. */
#define SCSI_ASENSE_INVALID_COMMAND                    0x20 		/** SCSI Additional Sense Code to indicate an invalid command was issued. */
#define SCSI_ASENSE_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE 0x21 		/** SCSI Additional Sense Code to indicate a write to a block out outside of the medium's range was issued. */
#define SCSI_ASENSE_MEDIUM_NOT_PRESENT                 0x3A		/** SCSI Additional Sense Code to indicate that no removable medium is inserted into the device. */

/** SCSI Additional Sense Key Code Qualifiers
 */
#define SCSI_ASENSEQ_NO_QUALIFIER                      0x00 		/** SCSI Additional Sense Qualifier Code to indicate no additional sense qualifier information is available. */
#define SCSI_ASENSEQ_FORMAT_COMMAND_FAILED             0x01 		/** SCSI Additional Sense Qualifier Code to indicate that a medium format command failed to complete. */

/** SCSI Additional Sense Qualifier Code to indicate that an initializing command must be issued before the issued
 *  command can be executed.
 */
#define SCSI_ASENSEQ_INITIALIZING_COMMAND_REQUIRED     0x02

/** SCSI Additional Sense Qualifier Code to indicate that an operation is currently in progress.
 */
#define SCSI_ASENSEQ_OPERATION_IN_PROGRESS             0x07



/** Enum for possible Class, Subclass and Protocol values of device and interface descriptors relating to the Mass
 *  Storage device class.
 */
enum MS_Descriptor_ClassSubclassProtocol_t
{ MS_CSCP_MassStorageClass         = 0x08 /**< Descriptor Class value indicating that the device or interface			                                           *   belongs to the Mass Storage class.			                                           */
,	MS_CSCP_SCSITransparentSubclass  = 0x06 /**< Descriptor Subclass value indicating that the device or interface			                                           *   belongs to the SCSI Transparent Command Set subclass of the Mass			                                           *   storage class.			                                           */
, MS_CSCP_BulkOnlyTransportProtocol= 0x50 /**< Descriptor Protocol value indicating that the device or interface			                                           *   belongs to the Bulk Only Transport protocol of the Mass Storage class.			                                           */
};

/** Enum for the Mass Storage class specific control requests that can be issued by the USB bus host.
 */
enum MS_ClassRequests_t
{ MS_GET_MAX_LUN     = 0xFE /** Retrieve the total number of Logical Units (drives) in the SCSI device. */
,	MS_RESET_MASS_STOR = 0xFF /** Reset the Mass Storage interface ready for the next command.		          */
};

		/** Enum for the possible command status wrapper return status codes. */
enum MS_CommandStatusCodes_t
{ MS_SCSI_COMMAND_PASS      = 0 /**< Command completed with no error */
,	MS_SCSI_COMMAND_FAIL      = 1 /**< Command failed to complete - host may check the exact error via a			                                        *   SCSI REQUEST SENSE command.			                                        */
,	MS_SCSI_COMMAND_PhaseError= 2 /**< Command failed due to being invalid in the current phase. */
};

	/* Type Defines: */
/** &brief Mass Storage Class Command Block Wrapper.
 *
 *  Type define for a Command Block Wrapper, used in the Mass Storage Bulk-Only Transport protocol.
 *
 *  Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{ dword dSignature;     /** Command block signature, must be \ref MS_CBW_SIGNATURE to indicate a valid Command Block. */
	 dword dTag;           /** Unique command ID value, to associate a command block wrapper with its command status wrapper. */
	 dword dDataLength;    /** Length of the optional data portion of the issued command, in bytes. */
	 byte  bFlags;         /** Command block flags, indicating command data direction. */
	 byte  bLUN;           /** Logical Unit number this command is issued to. */
	 byte  bCmdLength;     /** Length of the issued SCSI command within the SCSI command data array. */
	 byte  bCmdData[ 16 ]; /** Issued SCSI command in the Command Block. */
} PACKED MScmdBlkWrapperRec;

/** &brief Mass Storage Class Command Status Wrapper.
 *
 *  Type define for a Command Status Wrapper, used in the Mass Storage Bulk-Only Transport protocol.
 *
 *  Regardless of CPU architecture, these values should be stored as little endian.
 */
typedef struct
{ dword dSignature;     /**< Status block signature, must be \ref MS_CSW_SIGNATURE to indicate a valid Command Status. */
	 dword dTag;          /**< Unique command ID value, to associate a command block wrapper with its command status wrapper. */
	 dword dDataResidue;  /**< Number of bytes of data not processed in the SCSI command. */
	 byte  bStatus;       /**< Status code of the issued command - a value from the \ref MS_CommandStatusCodes_t enum. */
} PACKED MScmdStatWrapperRec;

/** &brief Mass Storage Class SCSI Sense Structure
 *
 *  Type define for a SCSI Sense structure. Structures of this type are filled out by the
 *  device via the \ref MS_Host_RequestSense() function, indicating the current sense data of the
 *  device (giving explicit error codes for the last issued command). For details of the
 *  structure contents, refer to the SCSI specifications.
 */

typedef struct
{ byte  ResponseCode;

		byte  SegmentNumber;

		unsigned SenseKey : 4;
		unsigned Reserved : 1;
		unsigned ILI      : 1;
		unsigned EOM      : 1;
		unsigned FileMark : 1;

		byte  Information[4];
		byte  AdditionalLength;
		byte  CmdSpecificInformation[4];
		byte  AdditionalSenseCode;
		byte  AdditionalSenseQualifier;
		byte  FieldReplaceableUnitCode;
		byte  SenseKeySpecific[3];

} PACKED SCSI_Request_Sense_Response_t;


#define DEVICE_TYPE_BLOCK   0x00 /** Value for the DeviceType entry in the SCSI_Inquiry_Response_t enum, indicating a Block Media device. */

#define TOTAL_LUNS                 1
#define VIRTUAL_MEMORY_BYTES       ((dword)16384) 		/** Total number of bytes of the storage medium, comprised of one or more Dataflash ICs. */
#define VIRTUAL_MEMORY_BLOCK_SIZE   512		/** Block size of the device. This is kept at 512 to remain compatible with the OS despite the underlying		 *  storage media (Dataflash) using a different native block size. Do not change this value.		 */
#define VIRTUAL_MEMORY_BLOCKS      (VIRTUAL_MEMORY_BYTES / VIRTUAL_MEMORY_BLOCK_SIZE) 		/** Total number of blocks of the virtual memory for reporting to the host as the device's total capacity. Do not		 *  change this value; change VIRTUAL_MEMORY_BYTES instead to alter the media size.		 */
#define LUN_MEDIA_BLOCKS           (VIRTUAL_MEMORY_BLOCKS / TOTAL_LUNS) 		/** Blocks in each LUN, calculated from the total capacity divided by the total number of Logical Units in the device. */

/** Macro to set the current SCSI sense data to the given key, additional sense code and additional sense qualifier. This
 *  is for convenience, as it allows for all three sense values (returned upon request to the host to give information about
 *  the last command failure) in a quick and easy manner.
 *
 *  \param[in] Key    New SCSI sense key to set the sense code to
 *  \param[in] Acode  New SCSI additional sense key to set the additional sense code to
 *  \param[in] Aqual  New SCSI additional sense key qualifier to set the additional sense qualifier code to
 */
#define SCSI_SET_SENSE( Key, Acode, Aqual) do { SenseData.SenseKey                 = (Key);   \
		                                              SenseData.AdditionalSenseCode      = (Acode); \
		                                              SenseData.AdditionalSenseQualifier = (Aqual); } while (0)

/**    Mass Storage Class SCSI Inquiry Structure.
 *
 *  Type define for a SCSI Inquiry structure. Structures of this type are filled out by the
 *  device via the \ref MS_Host_GetInquiryData() function, retrieving the attached device's
 *  information.
 */

typedef struct
{ unsigned DeviceType          : 5;
		unsigned PeripheralQualifier : 3;

		unsigned Reserved            : 7;
		unsigned Removable           : 1;

		byte  Version;

		unsigned ResponseDataFormat  : 4;
		unsigned Reserved2           : 1;
		unsigned NormACA             : 1;
		unsigned TrmTsk              : 1;
		unsigned AERC                : 1;

		byte  AdditionalLength;
		byte  Reserved3[2];

		unsigned SoftReset    : 1;
		unsigned CmdQue       : 1;
		unsigned Reserved4    : 1;
		unsigned Linked       : 1;
		unsigned Sync         : 1;
		unsigned WideBus16Bit : 1;
		unsigned WideBus32Bit : 1;
		unsigned RelAddr      : 1;

		byte  VendorID[   8 ];
		byte  ProductID[ 16 ];
		byte  RevisionID[ 4 ];
} PACKED SCSI_Inquiry_Response_t;

/**   Mass Storage Class Device Mode Configuration and State Structure.
 *
 *  Class state structure. An instance of this structure should be made for each Mass Storage interface
 *  within the user application, and passed to each of the Mass Storage class driver functions as the
 *  MSInterfaceInfo parameter. This stores each Mass Storage interface's configuration and state information.
 */
//typedef struct
//{
 // struct
//		{ byte  InterfaceNumber;                /**< Interface number of the Mass Storage interface within the device. */
// 			USB_Endpoint_Table_t DataINEndpoint;  /**< Data IN endpoint configuration table. */
//	 		USB_Endpoint_Table_t DataOUTEndpoint; /**< Data OUT endpoint configuration table. */
//				byte  TotalLUNs;                      /**< Total number of logical drives in the Mass Storage interface. */
//		} Config;  /**< Config data for the USB class interface within the device. All elements in this section				           *   <b>must</b> be set or the interface will fail to enumerate and operate correctly.				           */

//		struct
//		{ MScmdBlkWrapperRec  CommandBlock; /**< Mass Storage class command block structure, stores the received SCSI															 *   command from the host which is to be processed.															 */
//				MScmdStatWrapperRec CommandStatus; /**< Mass Storage class command status structure, set elements to indicate															  *   the issued command's success or failure to the host.															  */

// 			volatile bool IsMassStoreReset; /**< Flag indicating that the host has requested that the Mass Storage interface be reset											         *   and that all current Mass Storage operations should immediately abort.											         */
//
//		} State; /**< State data for the USB class interface within the device. All elements in this section				          *   are reset to their defaults when the interface is enumerated.				          */
//}

// MSclassDeviceRec;




#endif

