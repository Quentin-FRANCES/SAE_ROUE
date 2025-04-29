onerror {quit -f}
vlib work
vlog -work work pld_mbed_etudiant.vo
vlog -work work pld_mbed_etudiant.vt
vsim -novopt -c -t 1ps -L max7000s_ver -L altera_ver -L altera_mf_ver -L 220model_ver -L sgate work.filtre_hall_vlg_vec_tst
vcd file -direction pld_mbed_etudiant.msim.vcd
vcd add -internal filtre_hall_vlg_vec_tst/*
vcd add -internal filtre_hall_vlg_vec_tst/i1/*
add wave /*
run -all
