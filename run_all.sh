clear
make
for a in  apte xerox hp ami33 ami49 
# for a in  ami49 
do

    for r in 0.25 0.5 0.75
    do

        ./bin/fp ${r} ../input_pa2/${a}.block ../input_pa2/${a}.nets ../output/${a}.rpt
        ../checker/checker ../input_pa2/${a}.block ../input_pa2/${a}.nets ../sample/output.rpt ${r}

    done
done

