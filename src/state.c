bool saveState(void) {
    FILE *f = fopen(".state", "wb");
    if (f == NULL) {
        return false;
    }

    fwrite(&state, 1, sizeof(GameState), f);

    fclose(f);
    return true;
}

bool loadState(void) {
    FILE *f = fopen(".state", "rb");
    if (f == NULL) {
        return false;
    }

    fread(&state, 1, sizeof(GameState), f);
    fclose(f);
    return true;
}
