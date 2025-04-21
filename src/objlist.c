void deleteNode(ObjectNode **node) {
    ObjectNode *replacement = NULL;

    if ((*node)->prev != NULL && (*node)->next != NULL) {
        (*node)->prev->next = (*node)->next;
        (*node)->next->prev = (*node)->prev;
        replacement = (*node)->next;
    } else if ((*node)->prev != NULL) {
        (*node)->prev->next = NULL;
        replacement = (*node)->prev;
    } else if ((*node)->next != NULL) {
        (*node)->next->prev = NULL;
        replacement = (*node)->next;
    }

    free((*node));
    (*node) = replacement;
}

void objectHeadRewind() {
    if (state.objectHead == NULL) return;

    while (state.objectHead->prev != NULL) {
        state.objectHead = state.objectHead->prev;
    }
}

void objectHeadForward() {
    if (state.objectHead == NULL) return;

    while (state.objectHead->next != NULL) {
        state.objectHead = state.objectHead->next;
    }
}
