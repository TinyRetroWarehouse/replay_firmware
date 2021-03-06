/**
 * \file
 *
 * \brief USB hub protocol definitions.
 *
 * Copyright (c) 2016-2018 Microchip Technology Inc. and its subsidiaries.
 *
 * \asf_license_start
 *
 * \asf_license_start
 *
 * \page License
 *
 * Subject to your compliance with these terms, you may use Microchip
 * software and any derivatives exclusively with Microchip products.
 * It is your responsibility to comply with third party license terms applicable
 * to your use of third party software (including open source software) that
 * may accompany Microchip software.
 *
 * THIS SOFTWARE IS SUPPLIED BY MICROCHIP "AS IS". NO WARRANTIES,
 * WHETHER EXPRESS, IMPLIED OR STATUTORY, APPLY TO THIS SOFTWARE,
 * INCLUDING ANY IMPLIED WARRANTIES OF NON-INFRINGEMENT, MERCHANTABILITY,
 * AND FITNESS FOR A PARTICULAR PURPOSE. IN NO EVENT WILL MICROCHIP BE
 * LIABLE FOR ANY INDIRECT, SPECIAL, PUNITIVE, INCIDENTAL OR CONSEQUENTIAL
 * LOSS, DAMAGE, COST OR EXPENSE OF ANY KIND WHATSOEVER RELATED TO THE
 * SOFTWARE, HOWEVER CAUSED, EVEN IF MICROCHIP HAS BEEN ADVISED OF THE
 * POSSIBILITY OR THE DAMAGES ARE FORESEEABLE.  TO THE FULLEST EXTENT
 * ALLOWED BY LAW, MICROCHIP'S TOTAL LIABILITY ON ALL CLAIMS IN ANY WAY
 * RELATED TO THIS SOFTWARE WILL NOT EXCEED THE AMOUNT OF FEES, IF ANY,
 * THAT YOU HAVE PAID DIRECTLY TO MICROCHIP FOR THIS SOFTWARE.
 *
 * \asf_license_stop
 *
 * \asf_license_stop
 *
 */

#ifndef _USB_PROTOCOL_HUB_H_
#define _USB_PROTOCOL_HUB_H_

#include "usb_protocol.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \name Possible class value */
/*@{*/
#define HUB_CLASS 0x09
/*@}*/

/** \brief Hub USB requests (bRequest) */
enum usb_reqid_hub {
	USB_REQ_HUB_GET_STATUS = 0,
	/** CLEAR_FEATURE */
	USB_REQ_HUB_CLR_FTR = 1,
	/** SET_FEATURE */
	USB_REQ_HUB_SET_FTR = 3,
	/** GET_DESCRIPTOR */
	USB_REQ_HUB_GET_DESC = 6,
	/** SET_DESCRIPTOR */
	USB_REQ_HUB_SET_DESC = 7,
	/** CLEAR_TT_BUFFER */
	USB_REQ_HUB_CLR_TT_BUF = 8,
	/** RESET_TT */
	USB_REQ_HUB_RESET_TT = 9,
	/** GET_TT_STATE */
	USB_REQ_HUB_GET_TT_STATE = 10,
	/** STOP_TT */
	USB_REQ_HUB_STOP_TT
};

/** \brief Hub USB descriptor type */
enum usb_descriptor_type_hub { USB_DT_HUB = 0x29 };

/** \brief Hub feature selector */
enum usb_hub_ftr_sel {
	USB_HUB_FTR_C_HUB_LOCAL_POWER   = 0,
	USB_HUB_FTR_C_HUB_OVER_CURRENT  = 1,
	USB_HUB_FTR_PORT_CONNECTION     = 0,
	USB_HUB_FTR_PORT_ENABLE         = 1,
	USB_HUB_FTR_PORT_SUSPEND        = 2,
	USB_HUB_FTR_PORT_OVER_CURRENT   = 3,
	USB_HUB_FTR_PORT_RESET          = 4,
	USB_HUB_FTR_PORT_POWER          = 8,
	USB_HUB_FTR_PORT_LOW_SPEED      = 9,
	USB_HUB_FTR_PORT_LS             = 9,
	USB_HUB_FTR_PORT_HIGH_SPEED     = 10,
	USB_HUB_FTR_PORT_HS             = 10,
	USB_HUB_FTR_C_PORT_CONNECTION   = 16,
	USB_HUB_FTR_C_PORT_ENABLE       = 17,
	USB_HUB_FTR_C_PORT_SUSPEND      = 18,
	USB_HUB_FTR_C_PORT_OVER_CURRENT = 19,
	USB_HUB_FTR_C_PORT_RESET        = 20,
	USB_HUB_FTR_PORT_TEST           = 21,
	USB_HUB_FTR_PORT_INDICATOR
};

COMPILER_PACK_SET(1)

/**
 * \brief General Hub Descriptor
 */
typedef struct usb_hub_descriptor {
	uint8_t  bDescLength;
	uint8_t  bDescriptorType;
	uint8_t  bNbrPorts;
	uint16_t wHubCharacteristics;
	uint8_t  bPwrOn2PwrGood;
	uint8_t  bHubContrCurrent;
	uint32_t variables[1];
} usb_hub_descriptor_t;

/**
 * \brief Hub Descriptor with no more than 7 ports
 */
typedef struct usb_hub_descriptor_7p {
	uint8_t  bDescLength;
	uint8_t  bDescriptorType;
	uint8_t  bNbrPorts;
	uint16_t wHubCharacteristics;
	uint8_t  bPwrOn2PwrGood;
	uint8_t  bHubContrCurrent;
	uint8_t  DeviceRemovable[1];
	uint8_t  PortPwrCtrlMask[1];
} usb_hub_descriptor_7p_t;

/**
 * \brief Hub Descriptor with no more than 15 ports
 */
typedef struct usb_hub_descriptor_15p {
	uint8_t  bDescLength;
	uint8_t  bDescriptorType;
	uint8_t  bNbrPorts;
	uint16_t wHubCharacteristics;
	uint8_t  bPwrOn2PwrGood;
	uint8_t  bHubContrCurrent;
	uint8_t  DeviceRemovable[2];
	uint8_t  PortPwrCtrlMask[2];
} usb_hub_descriptor_15p_t;

/**
 * \brief Hub Descriptor with no more than 23 ports
 */
typedef struct usb_hub_descriptor_23p {
	uint8_t  bDescLength;
	uint8_t  bDescriptorType;
	uint8_t  bNbrPorts;
	uint16_t wHubCharacteristics;
	uint8_t  bPwrOn2PwrGood;
	uint8_t  bHubContrCurrent;
	uint8_t  DeviceRemovable[3];
	uint8_t  PortPwrCtrlMask[3];
} usb_hub_descriptor_23p_t;

/**
 * \brief Hub Descriptor with no more than 31 ports
 */
typedef struct usb_hub_descriptor_31p {
	uint8_t  bDescLength;
	uint8_t  bDescriptorType;
	uint8_t  bNbrPorts;
	uint16_t wHubCharacteristics;
	uint8_t  bPwrOn2PwrGood;
	uint8_t  bHubContrCurrent;
	uint8_t  DeviceRemovable[4];
	uint8_t  PortPwrCtrlMask[4];
} usb_hub_descriptor_31p_t;

COMPILER_PACK_RESET()

/**
 * \brief Check if hub change is detected
 * \param[in] bitmap Pointer to the bitmap location
 * \return \c true if hub change is detected
 */
static inline bool usb_is_hub_change_detected(const uint8_t *bitmap)
{
	return (*bitmap & 0x1);
}

/**
 * \brief Check if port change is detected
 * \param[in] bitmap Pointer to the bitmap location
 * \param[in] port   Port number ( 1 ~ n )
 * \return \c true if port change is detected
 */
static inline bool usb_is_port_change_detected(const uint8_t *bitmap, const uint8_t port)
{
	uint8_t idx = port >> 3;
	uint8_t pos = port & 0x7;
	return (bitmap[idx] & (1u << pos));
}

/**
 * \brief Fill a ClearHubFeature request
 * \param[out] req Pointer to the request to fill
 * \param[in]  ftr Feature selector
 */
static inline void usb_fill_ClearHubFeature_req(usb_setup_req_t *req, uint8_t ftr)
{
	req->bmRequestType = 0x20;
	req->bRequest      = USB_REQ_CLEAR_FEATURE;
	req->wValue        = ftr;
	req->wIndex        = 0;
	req->wLength       = 0;
}

/**
 * \brief Fill a ClearPortFeature request
 * \param[out] req  Pointer to the request to fill
 * \param[in]  ftr  Feature selector
 * \param[in]  port Port number ( 1 ~ n )
 */
static inline void usb_fill_ClearPortFeature_req(usb_setup_req_t *req, uint8_t ftr, uint8_t port)
{
	req->bmRequestType = 0x23;
	req->bRequest      = USB_REQ_CLEAR_FEATURE;
	req->wValue        = ftr;
	req->wIndex        = port;
	req->wLength       = 0;
}

/**
 * \brief Fill a ClearTTBuffer request
 * \param[out] req     Pointer to the request to fill
 * \param[in]  dev     Dev_Addr
 * \param[in]  ep      EP_Num
 * \param[in]  tt_port TT_port
 */
static inline void usb_fill_ClrTTBuf_req(usb_setup_req_t *req, uint8_t dev, uint8_t ep, uint8_t tt_port)
{
	req->bmRequestType = 0x23;
	req->bRequest      = USB_REQ_HUB_CLR_TT_BUF;
	req->wValue        = (dev << 8) | ep;
	req->wIndex        = tt_port;
	req->wLength       = 0;
}

/**
 * \brief Fill a GetHubDescriptor request
 * \param[out] req Pointer to the request to fill
 * \param[in]  len Length of available buffer for request data
 */
static inline void usb_fill_GetHubDesc_req(usb_setup_req_t *req, uint16_t len)
{
	req->bmRequestType = 0xA0;
	req->bRequest      = USB_REQ_GET_DESCRIPTOR;
	req->wValue        = (USB_DT_HUB << 8) | 0;
	req->wIndex        = 0;
	req->wLength       = len;
}

/**
 * \brief Fill a GetHubStatus request
 * \param[out] req Pointer to the request to fill
 */
static inline void usb_fill_GetHubStatus_req(usb_setup_req_t *req)
{
	req->bmRequestType = 0xA0;
	req->bRequest      = USB_REQ_GET_STATUS;
	req->wValue        = 0;
	req->wIndex        = 0;
	req->wLength       = 4;
}

/**
 * \brief Fill a GetPortStatus request
 * \param[out] req  Pointer to the request to fill
 * \param[in]  port Port number ( 1 ~ n )
 */
static inline void usb_fill_GetPortStatus_req(usb_setup_req_t *req, uint8_t port)
{
	req->bmRequestType = 0xA3;
	req->bRequest      = USB_REQ_GET_STATUS;
	req->wValue        = 0;
	req->wIndex        = port;
	req->wLength       = 4;
}

/**
 * \brief Fill a ResetTT request
 * \param[out] req  Pointer to the request to fill
 * \param[in]  port Port number ( 1 ~ n )
 */
static inline void usb_fill_ResetTT_req(usb_setup_req_t *req, uint8_t port)
{
	req->bmRequestType = 0x23;
	req->bRequest      = USB_REQ_HUB_RESET_TT;
	req->wValue        = 0;
	req->wIndex        = port;
	req->wLength       = 0;
}

/**
 * \brief Fill a SetHubFeature request
 * \param[out] req  Pointer to the request to fill
 * \param[in]  ftr  Feature selector
 */
static inline void usb_fill_SetHubFeature_req(usb_setup_req_t *req, uint8_t ftr)
{
	req->bmRequestType = 0x20;
	req->bRequest      = USB_REQ_SET_FEATURE;
	req->wValue        = ftr;
	req->wIndex        = 0;
	req->wLength       = 0;
}

/**
 * \brief Fill a SetPortFeature request
 * \param[out] req  Pointer to the request to fill
 * \param[in]  ftr  Feature selector
 * \param[in]  port Port number ( 1 ~ n )
 */
static inline void usb_fill_SetPortFeature_req(usb_setup_req_t *req, uint8_t ftr, uint8_t port)
{
	req->bmRequestType = 0x23;
	req->bRequest      = USB_REQ_SET_FEATURE;
	req->wValue        = ftr;
	req->wIndex        = port;
	req->wLength       = 0;
}

/**
 * \brief Fill a GetTTState request
 * \param[out] req      Pointer to the request to fill
 * \param[in]  tt_flags TT_Flags
 * \param[in]  port     Port number ( 1 ~ n )
 * \param[in]  len      TT State Length
 */
static inline void usb_fill_GetTTState_req(usb_setup_req_t *req, uint16_t tt_flags, uint8_t port, uint16_t len)
{
	req->bmRequestType = 0xA3;
	req->bRequest      = USB_REQ_HUB_GET_TT_STATE;
	req->wValue        = tt_flags;
	req->wIndex        = port;
	req->wLength       = len;
}

/**
 * \brief Fill a StopTT request
 * \param[out] req      Pointer to the request to fill
 * \param[in]  port     Port number ( 1 ~ n )
 */
static inline void usb_fill_StopTT_req(usb_setup_req_t *req, uint8_t port)
{
	req->bmRequestType = 0x23;
	req->bRequest      = USB_REQ_HUB_STOP_TT;
	req->wValue        = 0;
	req->wIndex        = port;
	req->wLength       = 0;
}

#ifdef __cplusplus
}
#endif

#endif /* _USB_PROTOCOL_HUB_H_ */
