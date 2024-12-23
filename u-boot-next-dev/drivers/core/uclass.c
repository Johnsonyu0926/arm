// uclass.c

#include <common.h>
#include <dm.h>
#include <errno.h>
#include <malloc.h>
#include <dm/device.h>
#include <dm/device-internal.h>
#include <dm/lists.h>
#include <dm/uclass.h>
#include <dm/uclass-internal.h>
#include <dm/util.h>

DECLARE_GLOBAL_DATA_PTR;

/**
 * @brief Find a uclass by its ID
 *
 * @param key Uclass ID
 * @return Pointer to the uclass, or NULL if not found
 */
struct uclass *uclass_find(enum uclass_id key)
{
	struct uclass *uc;

	if (!gd->dm_root)
		return NULL;

	list_for_each_entry(uc, &gd->uclass_root, sibling_node) {
		if (uc->uc_drv->id == key)
			return uc;

		if (uc->uc_drv->id == UCLASS_ROOT)
			break;
	}

	return NULL;
}

/**
 * @brief Create a new uclass and add it to the list
 *
 * @param id Uclass ID
 * @param ucp Pointer to store the created uclass
 * @return 0 on success, otherwise error
 */
static int uclass_add(enum uclass_id id, struct uclass **ucp)
{
	struct uclass_driver *uc_drv;
	struct uclass *uc;
	int ret;

	*ucp = NULL;
	uc_drv = lists_uclass_lookup(id);
	if (!uc_drv) {
		debug("Cannot find uclass for id %d: please add the UCLASS_DRIVER() declaration for this UCLASS_... id\n",
		      id);
		return -EPFNOSUPPORT;
	}
	uc = calloc(1, sizeof(*uc));
	if (!uc)
		return -ENOMEM;
	if (uc_drv->priv_auto_alloc_size) {
		uc->priv = calloc(1, uc_drv->priv_auto_alloc_size);
		if (!uc->priv) {
			ret = -ENOMEM;
			goto fail_mem;
		}
	}
	uc->uc_drv = uc_drv;
#ifdef CONFIG_USING_KERNEL_DTB_V2
	uc->u_boot_dev_head = NULL;
#endif
	INIT_LIST_HEAD(&uc->sibling_node);
	INIT_LIST_HEAD(&uc->dev_head);
	list_add(&uc->sibling_node, &DM_UCLASS_ROOT_NON_CONST);

	if (uc_drv->init) {
		ret = uc_drv->init(uc);
		if (ret)
			goto fail;
	}

	*ucp = uc;

	return 0;
fail:
	if (uc_drv->priv_auto_alloc_size) {
		free(uc->priv);
		uc->priv = NULL;
	}
	list_del(&uc->sibling_node);
fail_mem:
	free(uc);

	return ret;
}

/**
 * @brief Destroy a uclass
 *
 * @param uc Uclass to destroy
 * @return 0 on success, otherwise error
 */
int uclass_destroy(struct uclass *uc)
{
	struct uclass_driver *uc_drv;
	struct udevice *dev;
	int ret;

	while (!list_empty(&uc->dev_head)) {
		dev = list_first_entry(&uc->dev_head, struct udevice,
				       uclass_node);
		ret = device_remove(dev, DM_REMOVE_NORMAL);
		if (ret)
			return ret;
		ret = device_unbind(dev);
		if (ret)
			return ret;
	}

	uc_drv = uc->uc_drv;
	if (uc_drv->destroy)
		uc_drv->destroy(uc);
	list_del(&uc->sibling_node);
	if (uc_drv->priv_auto_alloc_size)
		free(uc->priv);
	free(uc);

	return 0;
}

/**
 * @brief Get a uclass by its ID
 *
 * @param id Uclass ID
 * @param ucp Pointer to store the uclass
 * @return 0 on success, otherwise error
 */
int uclass_get(enum uclass_id id, struct uclass **ucp)
{
	struct uclass *uc;

	*ucp = NULL;
	uc = uclass_find(id);
	if (!uc)
		return uclass_add(id, ucp);
	*ucp = uc;

	return 0;
}

/**
 * @brief Get the name of a uclass by its ID
 *
 * @param id Uclass ID
 * @return Uclass name, or NULL if not found
 */
const char *uclass_get_name(enum uclass_id id)
{
	struct uclass *uc;

	if (uclass_get(id, &uc))
		return NULL;
	return uc->uc_drv->name;
}

/**
 * @brief Get the uclass ID by its name
 *
 * @param name Uclass name
 * @return Uclass ID, or UCLASS_INVALID if not found
 */
enum uclass_id uclass_get_by_name(const char *name)
{
	int i;

	for (i = 0; i < UCLASS_COUNT; i++) {
		struct uclass_driver *uc_drv = lists_uclass_lookup(i);

		if (uc_drv && !strcmp(uc_drv->name, name))
			return i;
	}

	return UCLASS_INVALID;
}

/**
 * @brief Find a device in a uclass by its index
 *
 * @param id Uclass ID
 * @param index Device index
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_device(enum uclass_id id, int index, struct udevice **devp)
{
	struct uclass *uc;
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;
	if (list_empty(&uc->dev_head))
		return -ENODEV;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		if (!index--) {
			*devp = dev;
			return 0;
		}
	}

	return -ENODEV;
}

/**
 * @brief Find the first device in a uclass
 *
 * @param id Uclass ID
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_first_device(enum uclass_id id, struct udevice **devp)
{
	struct uclass *uc;
	int ret;

	*devp = NULL;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;
	if (list_empty(&uc->dev_head))
		return 0;

	*devp = list_first_entry(&uc->dev_head, struct udevice, uclass_node);

	return 0;
}

/**
 * @brief Find the next device in a uclass
 *
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_next_device(struct udevice **devp)
{
	struct udevice *dev = *devp;

	*devp = NULL;
	if (list_is_last(&dev->uclass_node, &dev->uclass->dev_head))
		return 0;

	*devp = list_entry(dev->uclass_node.next, struct udevice, uclass_node);

	return 0;
}

/**
 * @brief Find a device in a uclass by its name
 *
 * @param id Uclass ID
 * @param name Device name
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_device_by_name(enum uclass_id id, const char *name,
			       struct udevice **devp)
{
	struct uclass *uc;
	struct udevice *dev;
	int ret;

	*devp = NULL;
	if (!name)
		return -EINVAL;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		if (!strncmp(dev->name, name, strlen(name))) {
			*devp = dev;
			return 0;
		}
	}

	return -ENODEV;
}

/**
 * @brief Find a device in a uclass by its sequence
 *
 * @param id Uclass ID
 * @param seq_or_req_seq Sequence or requested sequence
 * @param find_req_seq Find requested sequence flag
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_device_by_seq(enum uclass_id id, int seq_or_req_seq,
			      bool find_req_seq, struct udevice **devp)
{
	struct uclass *uc;
	struct udevice *dev;
	int ret;

	*devp = NULL;
	pr_debug("%s: %d %d\n", __func__, find_req_seq, seq_or_req_seq);
	if (seq_or_req_seq == -1)
		return -ENODEV;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		pr_debug("   - %d %d '%s'\n", dev->req_seq, dev->seq, dev->name);
		if ((find_req_seq ? dev->req_seq : dev->seq) ==
				seq_or_req_seq) {
			*devp = dev;
			pr_debug("   - found\n");
			return 0;
		}
	}
	pr_debug("   - not found\n");

	return -ENODEV;
}

/**
 * @brief Find a device in a uclass by its device tree offset
 *
 * @param id Uclass ID
 * @param node Device tree offset
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_device_by_of_offset(enum uclass_id id, int node,
				    struct udevice **devp)
{
	struct uclass *uc;
	struct udevice *dev;
	int ret;

	*devp = NULL;
	if (node < 0)
		return -ENODEV;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		if (dev_of_offset(dev) == node) {
			*devp = dev;
			return 0;
		}
	}

	return -ENODEV;
}

/**
 * @brief Find a device in a uclass by its device tree node
 *
 * @param id Uclass ID
 * @param node Device tree node
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_find_device_by_ofnode(enum uclass_id id, ofnode node,
				 struct udevice **devp)
{
	struct uclass *uc;
	struct udevice *dev;
	int ret;

	*devp = NULL;
	if (!ofnode_valid(node))
		return -ENODEV;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		if (ofnode_equal(dev_ofnode(dev), node)) {
			*devp = dev;
			return 0;
		}
	}

	return -ENODEV;
}

#if CONFIG_IS_ENABLED(OF_CONTROL)
/**
 * @brief Find a device in a uclass by its phandle
 *
 * @param id Uclass ID
 * @param parent Parent device
 * @param name Phandle name
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
static int uclass_find_device_by_phandle(enum uclass_id id,
					 struct udevice *parent,
					 const char *name,
					 struct udevice **devp)
{
	struct udevice *dev;
	struct uclass *uc;
	int find_phandle;
	int ret;

	*devp = NULL;
	find_phandle = dev_read_u32_default(parent, name, -1);
	if (find_phandle <= 0)
		return -ENOENT;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		uint phandle;

		phandle = dev_read_phandle(dev);

		if (phandle == find_phandle) {
			*devp = dev;
			return 0;
		}
	}

	return -ENODEV;
}
#endif

/**
 * @brief Get a device in a uclass by its driver
 *
 * @param id Uclass ID
 * @param find_drv Driver to find
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_driver(enum uclass_id id,
				const struct driver *find_drv,
				struct udevice **devp)
{
	struct udevice *dev;
	struct uclass *uc;
	int ret;

	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		if (dev->driver == find_drv)
			return uclass_get_device_tail(dev, 0, devp);
	}

	return -ENODEV;
}

/**
 * @brief Get the tail of a device in a uclass
 *
 * @param dev Device
 * @param ret Return value
 * @param devp Pointer to store the device
 * @return Return value
 */
int uclass_get_device_tail(struct udevice *dev, int ret, struct udevice **devp)
{
	if (ret)
		return ret;

	assert(dev);
	ret = device_probe(dev);
	if (ret)
		return ret;

	*devp = dev;

	return 0;
}

/**
 * @brief Get a device in a uclass by its index
 *
 * @param id Uclass ID
 * @param index Device index
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device(enum uclass_id id, int index, struct udevice **devp)
{
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_find_device(id, index, &dev);
	return uclass_get_device_tail(dev, ret, devp);
}

/**
 * @brief Get a device in a uclass by its name
 *
 * @param id Uclass ID
 * @param name Device name
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_name(enum uclass_id id, const char *name,
			      struct udevice **devp)
{
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_find_device_by_name(id, name, &dev);
	return uclass_get_device_tail(dev, ret, devp);
}

/**
 * @brief Get a device in a uclass by its sequence
 *
 * @param id Uclass ID
 * @param seq Sequence
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_seq(enum uclass_id id, int seq, struct udevice **devp)
{
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_find_device_by_seq(id, seq, false, &dev);
	if (ret == -ENODEV) {
		ret = uclass_find_device_by_seq(id, seq, true, &dev);
	}
	return uclass_get_device_tail(dev, ret, devp);
}

/**
 * @brief Get a device in a uclass by its device tree offset
 *
 * @param id Uclass ID
 * @param node Device tree offset
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_of_offset(enum uclass_id id, int node,
				   struct udevice **devp)
{
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_find_device_by_of_offset(id, node, &dev);
	return uclass_get_device_tail(dev, ret, devp);
}

/**
 * @brief Get a device in a uclass by its device tree node
 *
 * @param id Uclass ID
 * @param node Device tree node
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_ofnode(enum uclass_id id, ofnode node,
				struct udevice **devp)
{
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_find_device_by_ofnode(id, node, &dev);

	return uclass_get_device_tail(dev, ret, devp);
}

#if CONFIG_IS_ENABLED(OF_CONTROL)
/**
 * @brief Get a device in a uclass by its phandle ID
 *
 * @param id Uclass ID
 * @param phandle_id Phandle ID
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_phandle_id(enum uclass_id id, uint phandle_id,
				    struct udevice **devp)
{
	struct udevice *dev;
	struct uclass *uc;
	int ret;

	*devp = NULL;
	ret = uclass_get(id, &uc);
	if (ret)
		return ret;

	ret = -ENODEV;
	list_for_each_entry(dev, &uc->dev_head, uclass_node) {
		uint phandle;

		phandle = dev_read_phandle(dev);

		if (phandle == phandle_id) {
			*devp = dev;
			ret = 0;
			break;
		}
	}

	return uclass_get_device_tail(dev, ret, devp);
}

/**
 * @brief Get a device in a uclass by its phandle
 *
 * @param id Uclass ID
 * @param parent Parent device
 * @param name Phandle name
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_get_device_by_phandle(enum uclass_id id, struct udevice *parent,
				 const char *name, struct udevice **devp)
{
	struct udevice *dev;
	int ret;

	*devp = NULL;
	ret = uclass_find_device_by_phandle(id, parent, name, &dev);
	return uclass_get_device_tail(dev, ret, devp);
}
#endif

/**
 * @brief Get the next device in a uclass that probes successfully
 *
 * @param dev Device
 * @param devp Pointer to store the found device
 */
static void _uclass_next_device(struct udevice *dev, struct udevice **devp)
{
	for (; dev; uclass_find_next_device(&dev)) {
		if (!device_probe(dev))
			break;
	}
	*devp = dev;
}

/**
 * @brief Get the first device in a uclass that probes successfully
 *
 * @param id Uclass ID
 * @param devp Pointer to store
 * the found device
 */
void uclass_first_device(enum uclass_id id, struct udevice **devp)
{
	struct udevice *dev;
	uclass_find_first_device(id, &dev);
	_uclass_next_device(dev, devp);
}
/**
 * @brief Get the next device in a uclass that probes successfully
 *
 * @param devp Pointer to store the found device
 */
void uclass_next_device(struct udevice **devp)
{
	struct udevice *dev = *devp;
	uclass_find_next_device(&dev);
	_uclass_next_device(dev, devp);
}
/**
 * @brief Get the first device in a uclass and check for errors
 *
 * @param id Uclass ID
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_first_device_err(enum uclass_id id, struct udevice **devp)
{
	int ret;
	ret = uclass_first_device_check(id, devp);
	if (ret)
		return ret;
	else if (!*devp)
		return -ENODEV;
	return 0;
}
/**
 * @brief Get the next device in a uclass and check for errors
 *
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_next_device_err(struct udevice **devp)
{
	int ret;
	ret = uclass_next_device_check(devp);
	if (ret)
		return ret;
	else if (!*devp)
		return -ENODEV;
	return 0;
}
/**
 * @brief Get the first device in a uclass and check for errors
 *
 * @param id Uclass ID
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_first_device_check(enum uclass_id id, struct udevice **devp)
{
	int ret;
	*devp = NULL;
	ret = uclass_find_first_device(id, devp);
	if (ret)
		return ret;
	if (!*devp)
		return 0;
	return device_probe(*devp);
}
/**
 * @brief Get the next device in a uclass and check for errors
 *
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_next_device_check(struct udevice **devp)
{
	int ret;
	ret = uclass_find_next_device(devp);
	if (ret)
		return ret;
	if (!*devp)
		return 0;
	return device_probe(*devp);
}
/**
 * @brief Get the first device in a uclass with specific driver data
 *
 * @param id Uclass ID
 * @param driver_data Driver data
 * @param devp Pointer to store the found device
 * @return 0 on success, otherwise error
 */
int uclass_first_device_drvdata(enum uclass_id id, ulong driver_data,
				struct udevice **devp)
{
	struct udevice *dev;
	struct uclass *uc;
	uclass_id_foreach_dev(id, dev, uc) {
		if (dev_get_driver_data(dev) == driver_data) {
			*devp = dev;
			return device_probe(dev);
		}
	}
	return -ENODEV;
}
/**
 * @brief Bind a device to a uclass
 *
 * @param dev Device to bind
 * @param after_u_boot_dev Flag indicating whether to bind after U-Boot devices
 * @return 0 on success, otherwise error
 */
int uclass_bind_device(struct udevice *dev, bool after_u_boot_dev)
{
	struct uclass *uc;
	int ret;
	uc = dev->uclass;
#ifdef CONFIG_USING_KERNEL_DTB_V2
	if (after_u_boot_dev)
		list_add_tail(&dev->uclass_node, &uc->dev_head);
	else
		list_add_tail(&dev->uclass_node, uc->u_boot_dev_head);
#else
	list_add_tail(&dev->uclass_node, &uc->dev_head);
#endif
	if (dev->parent) {
		struct uclass_driver *uc_drv = dev->parent->uclass->uc_drv;
		if (uc_drv->child_post_bind) {
			ret = uc_drv->child_post_bind(dev);
			if (ret)
				goto err;
		}
	}
	return 0;
err:
	/* There is no need to undo the parent's post_bind call */
	list_del(&dev->uclass_node);
	return ret;
}
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
/**
 * @brief Unbind a device from a uclass
 *
 * @param dev Device to unbind
 * @return 0 on success, otherwise error
 */
int uclass_unbind_device(struct udevice *dev)
{
	struct uclass *uc;
	int ret;
	uc = dev->uclass;
	if (uc->uc_drv->pre_unbind) {
		ret = uc->uc_drv->pre_unbind(dev);
		if (ret)
			return ret;
	}
	list_del(&dev->uclass_node);
	return 0;
}
#endif
/**
 * @brief Resolve the sequence number for a device in a uclass
 *
 * @param dev Device
 * @return Sequence number on success, otherwise error
 */
int uclass_resolve_seq(struct udevice *dev)
{
	struct udevice *dup;
	int seq;
	int ret;
	assert(dev->seq == -1);
	ret = uclass_find_device_by_seq(dev->uclass->uc_drv->id, dev->req_seq,
					false, &dup);
	if (!ret) {
		dm_warn("Device '%s': seq %d is in use by '%s'\n",
			dev->name, dev->req_seq, dup->name);
	} else if (ret == -ENODEV) {
		/* Our requested sequence number is available */
		if (dev->req_seq != -1)
			return dev->req_seq;
	} else {
		return ret;
	}
	for (seq = 0; seq < DM_MAX_SEQ; seq++) {
		ret = uclass_find_device_by_seq(dev->uclass->uc_drv->id, seq,
						false, &dup);
		if (ret == -ENODEV)
			break;
		if (ret)
			return ret;
	}
	return seq;
}
/**
 * @brief Pre-probe a device in a uclass
 *
 * @param dev Device
 * @return 0 on success, otherwise error
 */
int uclass_pre_probe_device(struct udevice *dev)
{
	struct uclass_driver *uc_drv;
	int ret;
	uc_drv = dev->uclass->uc_drv;
	if (uc_drv->pre_probe) {
		ret = uc_drv->pre_probe(dev);
		if (ret)
			return ret;
	}
	if (!dev->parent)
		return 0;
	uc_drv = dev->parent->uclass->uc_drv;
	if (uc_drv->child_pre_probe)
		return uc_drv->child_pre_probe(dev);
	return 0;
}
/**
 * @brief Post-probe a device in a uclass
 *
 * @param dev Device
 * @return 0 on success, otherwise error
 */
int uclass_post_probe_device(struct udevice *dev)
{
	struct uclass_driver *uc_drv = dev->uclass->uc_drv;
	if (uc_drv->post_probe)
		return uc_drv->post_probe(dev);
	return 0;
}
#if CONFIG_IS_ENABLED(DM_DEVICE_REMOVE)
/**
 * @brief Pre-remove a device in a uclass
 *
 * @param dev Device
 * @return 0 on success, otherwise error
 */
int uclass_pre_remove_device(struct udevice *dev)
{
	struct uclass *uc;
	int ret;
	uc = dev->uclass;
	if (uc->uc_drv->pre_remove) {
		ret = uc->uc_drv->pre_remove(dev);
		if (ret)
			return ret;
	}
	return 0;
}
#endif
UCLASS_DRIVER(nop) = {
	.id		= UCLASS_NOP,
	.name		= "nop",
};
//GST