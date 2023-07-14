include $(TOPDIR)/rules.mk

PKG_NAME:=esp-devices
PKG_VERSION:=1.0
PKG_RELEASE:=1
PKG_BUILD_DIR:=$(BUILD_DIR)/$(PKG_NAME)-$(PKG_VERSION)

include $(INCLUDE_DIR)/package.mk

define Package/esp-devices
	DEPENDS:=+libTuya +libubus +libubox +libblobmsg-json +libserialport
	SECTION:=utils
	CATEGORY:=Utilities
	TITLE:=UBUS module for controlling ESP micro controller
endef

define Package/esp-devices/install
	$(INSTALL_DIR) $(1)/usr/bin
	$(INSTALL_DIR) $(1)/etc/init.d
	$(INSTALL_BIN) $(PKG_BUILD_DIR)/esp_devices $(1)/usr/bin/esp_devices
	$(INSTALL_BIN) ./files/esp_devices.init $(1)/etc/init.d/esp_devices
	$(INSTALL_BIN) ./files/esp_devices_script $(1)/etc/init.d/esp_devices_script
endef

$(eval $(call BuildPackage,esp-devices))
