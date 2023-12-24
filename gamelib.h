// Dichiarazioni delle funzioni da utilizzare all'interno del main()
    void imposta_gioco(void);
    void gioca(void);
    void termina_gioco(void);

    enum classe_giocatore{
        barbaro,
        nano,
        elfo,
        mago
    };

    enum Tipo_zona{
        corridoio,
        scala,
        sala_banchetto,
        magazzino,
        giardino,
        posto_guardia,
        prigione,
        cucina,
        armeria,
        tempio
    };

    enum Tipo_tesoro{
        nessun_tesoro,
        veleno,
        guarigione,
        doppia_guarigione
    };

    enum Tipo_porta{
        nessuna_porta,
        porta_normale,
        porta_da_scassinare
    };

    typedef struct Zona_segrete{
        struct Zona_segrete* zona_successiva;
        struct Zona_segrete* zona_precedente;
        enum Tipo_zona zona;
        enum Tipo_tesoro tesoro;
        enum Tipo_porta porta;
    }Zona_segrete;

    typedef struct Giocatore{
        char nome_giocatore[50];
        enum classe_giocatore classe;
        struct Zona_segrete* posizione;
        unsigned char p_vita;
        unsigned char dadi_attacco;
        unsigned char dadi_difesa;
        unsigned char mente;
        unsigned char potere_speciale;
    }Giocatore;
