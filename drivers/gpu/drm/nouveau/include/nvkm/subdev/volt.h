#ifndef __NVKM_VOLT_H__
#define __NVKM_VOLT_H__
#include <core/subdev.h>

#define MAX_DVFS_FREQS	256

struct nvkm_voltage {
	u32 uv;
	u8  vid;
};

struct nvkm_volt {
	struct nvkm_subdev base;

	int (*vid_get)(struct nvkm_volt *);
	int (*get)(struct nvkm_volt *);
	int (*vid_set)(struct nvkm_volt *, u8 vid);
	int (*set)(struct nvkm_volt *, u32 uv);
	int (*set_id)(struct nvkm_volt *, u8 id, int condition);
	int (*get_voltage_by_id)(struct nvkm_volt *, u8 vid);

	u8 vid_mask;
	u8 vid_nr;
	struct nvkm_voltage vid[MAX_DVFS_FREQS];
	struct mutex therm_lock;
};

static inline struct nvkm_volt *
nvkm_volt(void *obj)
{
	return (void *)nvkm_subdev(obj, NVDEV_SUBDEV_VOLT);
}

#define nvkm_volt_create(p, e, o, d)                                        \
	nvkm_volt_create_((p), (e), (o), sizeof(**d), (void **)d)
#define nvkm_volt_destroy(p) ({                                             \
	struct nvkm_volt *v = (p);                                          \
	_nvkm_volt_dtor(nv_object(v));                                      \
})
#define nvkm_volt_init(p) ({                                                \
	struct nvkm_volt *v = (p);                                          \
	_nvkm_volt_init(nv_object(v));                                      \
})
#define nvkm_volt_fini(p,s)                                                 \
	nvkm_subdev_fini((p), (s))

int  nvkm_volt_create_(struct nvkm_object *, struct nvkm_object *,
			  struct nvkm_oclass *, int, void **);
void _nvkm_volt_dtor(struct nvkm_object *);
int  _nvkm_volt_init(struct nvkm_object *);
#define _nvkm_volt_fini _nvkm_subdev_fini

extern struct nvkm_oclass nv40_volt_oclass;
extern struct nvkm_oclass gk20a_volt_oclass;
extern struct nvkm_oclass gm20b_volt_oclass;

int nvkm_voltgpio_init(struct nvkm_volt *);
int nvkm_voltgpio_get(struct nvkm_volt *);
int nvkm_voltgpio_set(struct nvkm_volt *, u8);
#endif
