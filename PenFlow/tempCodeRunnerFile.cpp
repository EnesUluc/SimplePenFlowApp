 Fl_Input *inputCenter = new Fl_Input(710, 120, 80, 30);
    const char* userInput = inputCenter->value(); // value() çağrısı burada doğru           // Hata burada düzeltildi
    inputCenter->align(FL_ALIGN_RIGHT);
    getRadius = atoi(userInput);   