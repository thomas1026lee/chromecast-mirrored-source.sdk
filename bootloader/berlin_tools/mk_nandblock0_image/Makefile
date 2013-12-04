
all:mk_nandblock0_image
	@./mk_nandblock0_image block0.img

.PHONY: mk_nandblock0_image
mk_nandblock0_image: mk_ddr_device_timing.sh ./mk_nand_timing.sh 
	@./mk_ddr_device_timing.sh &&\
		./mk_nand_timing.sh || exit 1
	@if [ $(CPU_TYPE) = "Z1" -o $(CPU_TYPE) = "Z2" ]; then \
		gcc mk_nandblock0_image.c -o $@; \
	 else \
		gcc mk_nandblock0_image_A0.c -o $@; \
	 fi

clean:
	@rm -rf mk_nandblock0_image
	@rm -rf nand_param.h
	@rm -rf ddr_timing.h
	@rm -rf nand_block0.img
	@rm -rf block0.img
