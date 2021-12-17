`ifndef _main_
`define _main_

`include "header.sv"

`include "ctrl_bus.sv"
`include "motor_bus.sv"
`include "enc_bus.sv"
`include "adc_bus.sv"
`include "my_types.sv"

module main #(parameter
	KEYS = 32,
	ADC_WIDTH = 10,
		
	MTR_BAR	= 'h0,	MTR_MSK	= 'hFF,
	ENC_BAR	= 'h100,	ENC_MSK	= 'h7F,
	CTRL_BAR	= 'h180,	CTRL_MSK	= 'h3F,	
	ADC_BAR	= 'h1C0,	ADC_MSK	= 'h1F
)(
	input clk, aclr,
	
	input [15:0] rdaddr, wraddr,
	input [1:0] be,
	input write,
	input [15:0] wrdata,
	output reg [15:0] rddata,
	output nirq,
	
	output [7:0] step, dir,
	output sd_ena, sd_oe_n,
	output [4:0] X, Y,
	output [2:0] U, V,
	
	input [7:0] enc_A, enc_B, enc_Z,
	
	output adc_sclk, adc_csn, adc_mosi,
	input adc_miso,
	
	// OUT
	output [15:0] sig_out,
	
	// HV out
	output gen_sclk, gen_sdo, gen_lock,
	input gen_sdi,
	
	// Pult out
	output pult_sclk, pult_sdo, pult_lock,
	input pult_sdi,
	
	// Limit switches
	input [7:0] sig_in,
	input power_OK, wire_break,
	
	output led,
	output oe, center_n
);
	wire sclr;
	wire alarm, mtr_timeout;
	wire cut_permit, cstop;
	
	wire [15:0] ctrl_rddata, mtr_data, adc_rddata, enc_rddata;	
	wire [5:0] sem;
	wire hv_enabled;
	wire oi, run;	
	wire global_snapshot;
	wire [1:0] enc_changed;

	motor_bus #(.BAR(MTR_BAR), .MASK(MTR_MSK)) mtr_inst(
		.clk, .aclr, .sclr,
		.rdaddr, .wraddr, .be, .write, .wrdata, .rddata(mtr_data),	
		.permit(cut_permit), .cstop, .hv_enabled, .alarm,
		.step, .dir, .sd_ena, .sd_oe_n, .X, .Y, .U, .V,
		.mtr_timeout,
		.oi, .any_run(run),
		.global_snapshot, .enc_changed
	);

	enc_bus #(.BAR(ENC_BAR), .MASK(ENC_MSK)) enc_inst(
		.clk, .aclr, .sclr,
		.rdaddr, .wraddr, .be, .write, .wrdata, .rddata(enc_rddata),
		.enc_A, .enc_B, .enc_Z,
		.global_snapshot, .enc_changed
	);
	
	ctrl_bus #(.BAR(CTRL_BAR), .MASK(CTRL_MSK)) ctrl_inst(
		.clk, .aclr, .sclr,
		
		.rdaddr, .wraddr, .be, .write, .wrdata, .rddata(ctrl_rddata), .nirq,
		
		.sig_out,
		.gen_sclk, .gen_sdo, .gen_lock, .gen_sdi,
		.pult_sclk, .pult_sdo, .pult_lock, .pult_sdi,
		.power_OK, .wire_break, .sig_in,
		
		.mtr_permit(cut_permit), .mtr_timeout, .cstop,
		.alarm,		
		.led,
		.sem,
		.hv_enabled,
		.oe, .center_n
	);	

	adc_bus #(.ADC_WIDTH(ADC_WIDTH), .BAR(ADC_BAR), .MASK(ADC_MSK)) adc_inst(
		.clk, .aclr, .sclr,
		.rdaddr, .wraddr, .be, .write, .wrdata, .rddata(adc_rddata),		
		.oi, .run, .center(!center_n), .cut_permit, .cstop,
		.sclk(adc_sclk), .csn(adc_csn), .mosi(adc_mosi), .miso(adc_miso)		
	);
	
	always_ff @(posedge clk)
		rddata <= ctrl_rddata | mtr_data | adc_rddata | enc_rddata;
		
	// TEST
//	reg khz_ena, khz;
//	reg [31:0] khz_cnt;
//	
//	always_ff @(posedge clk, posedge aclr)
//		if (aclr)
//			khz_cnt <= '0;
//		else if (khz_ena)
//			khz_cnt <= '0;
//		else
//			khz_cnt <= khz_cnt + 1'b1;
//	
//	always_ff @(posedge clk, posedge aclr)
//		khz_ena <= aclr ? 1'b0 : khz_cnt == 32'(72_000_000 / 10_000 / 2 - 2);
//		
//	always_ff @(posedge clk, posedge aclr)
//		if (aclr)
//			khz <= 1'b0;
//		else if (khz_ena)
//			khz <= !khz;
//	
//	assign U[0] = khz;
//	assign U[2:1] = sem_ena ? sem[2:1] : _U[2:1];
	
endmodule :main

`endif
