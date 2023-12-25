# EMC-C
Erlang Module Generator in C
Program that will compile BEAM bytecode

# Resources

https://www.erlang.org/docs/26/pdf/otp-system-documentation.pdf

https://blog.stenmans.org/theBeamBook/#SEC-BeamModulesCTE

- When loading an Erlang file, have to compile first and specify an Erlang file.

`c("hello.erl")`

- https://www.erlang.org/doc/man/code#load_file-1

- This will load the Erlang with the code path
- Cannot load because it needs a good file header

### Errors that make occur when loading the BEAM file
```c 
read_result = beamfile_read(code,
                                unloaded_size,
                                &stp->beam);

    switch(read_result) {
    case BEAMFILE_READ_CORRUPT_FILE_HEADER:
        BeamLoadError0(stp, "corrupt file header");
    case BEAMFILE_READ_MISSING_ATOM_TABLE:
        BeamLoadError0(stp, "missing atom table");
    case BEAMFILE_READ_OBSOLETE_ATOM_TABLE:
        BeamLoadError0(stp, PLEASE_RECOMPILE);
    case BEAMFILE_READ_CORRUPT_ATOM_TABLE:
        BeamLoadError0(stp, "corrupt atom table");
    case BEAMFILE_READ_MISSING_CODE_CHUNK:
        BeamLoadError0(stp, "missing code chunk");
    case BEAMFILE_READ_CORRUPT_CODE_CHUNK:
        BeamLoadError0(stp, "corrupt code chunk");
    case BEAMFILE_READ_MISSING_EXPORT_TABLE:
        BeamLoadError0(stp, "missing export table");
    case BEAMFILE_READ_CORRUPT_EXPORT_TABLE:
        BeamLoadError0(stp, "corrupt export table");
    case BEAMFILE_READ_MISSING_IMPORT_TABLE:
        BeamLoadError0(stp, "missing import table");
    case BEAMFILE_READ_CORRUPT_IMPORT_TABLE:
        BeamLoadError0(stp, "corrupt import table");
    case BEAMFILE_READ_CORRUPT_LAMBDA_TABLE:
        BeamLoadError0(stp, "corrupt lambda table");
    case BEAMFILE_READ_CORRUPT_LINE_TABLE:
        BeamLoadError0(stp, "corrupt line table");
    case BEAMFILE_READ_CORRUPT_LITERAL_TABLE:
        BeamLoadError0(stp, "corrupt literal table");
    case BEAMFILE_READ_CORRUPT_LOCALS_TABLE:
        BeamLoadError0(stp, "corrupt locals table");
    case BEAMFILE_READ_CORRUPT_TYPE_TABLE:
        BeamLoadError0(stp, "corrupt type table");
    case BEAMFILE_READ_SUCCESS:
        break;
    }
```

