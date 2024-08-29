include $(TOPDIR)/rules.mk

PKG_NAME:=librf21x
PKG_VERSION:=1.3.0
PKG_RELEASE=$(PKG_SOURCE_VERSION)

PKG_SOURCE_PROTO:=hg
PKG_SOURCE_URL:=http://localhost:8000/
PKG_SOURCE_SUBDIR:=$(PKG_NAME)-$(PKG_VERSION)
PKG_SOURCE_VERSION:=tip
PKG_SOURCE:=$(PKG_NAME)-$(PKG_VERSION)-$(PKG_SOURCE_VERSION).tar.gz
CMAKE_INSTALL:=1

PKG_LICENSE:=GPLv2 LGPLv2.1
PKG_LICENSE_FILES:=

PKG_MAINTAINER:=GMiaow <miaow1988@qq.com>

include $(INCLUDE_DIR)/package.mk
include $(INCLUDE_DIR)/cmake.mk

define Package/librf21x
	SECTION:=libs
	CATEGORY:=Libraries
	TITLE:=EnjoyARS librf21x
	DEPENDS:=+libstdcpp +libusb-1.0 +libiconv-full +kmod-usb-hid
endef

CMAKE_OPTIONS:=-DCMAKE_INSTALL_PREFIX=/usr/bin -DUSB_UDEV=OFF -DBUILD_GSP_SERVER=ON

define Build/Compile
	$(MAKE) -C $(PKG_BUILD_DIR) gsp-server
endef

define Package/librf21x/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_BIN) $(PKG_INSTALL_DIR)/usr/bin/gsp-server $(1)/usr/bin/
endef

$(eval $(call BuildPackage,librf21x))