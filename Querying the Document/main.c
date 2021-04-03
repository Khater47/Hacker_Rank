#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<assert.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5


//my util variables
typedef struct{
    int paragraph;
    int sentences_num;
    int *words_num_in_sentences;
}info;

//my util functions
void check_null(void *pointer){
    if(pointer == NULL){
        printf("Null pointer found\n");
        exit(0);
    }
    return;
}

char* kth_word_in_mth_sentence_of_nth_paragraph(char**** document, int k, int m, int n) {
    return document[n][m][k];
}

char** kth_sentence_in_mth_paragraph(char**** document, int k, int m) { 
    return document[m][k];
}

char*** kth_paragraph(char**** document, int k) {
    return document[k];
}

char**** get_document(char* text) {
    char ****document = NULL;
    char buffer[MAX_CHARACTERS+1];
    int i, k, paragraph, sentence, word, character;
    info meta_data[MAX_PARAGRAPHS];
        
    //initialize meta_data
    for(i = 0; i < MAX_PARAGRAPHS; i++){
        meta_data[i].paragraph = i+1;
        meta_data[i].sentences_num = 0;
        meta_data[i].words_num_in_sentences = NULL;
    }
    
    //get number of sentences in each paragraph
    paragraph = sentence = character = 0;
    while(text[character] != '\0'){
        if(text[character] == '.'){
            meta_data[paragraph].sentences_num++;
        }else if(text[character] == '\n'){
            paragraph++;
        }
        character++;
    }
    //initialize word_num_in_sentences
    for(i = 0; i < 5; i++){
        meta_data[i].words_num_in_sentences = (int *)malloc(meta_data[i].sentences_num * sizeof(int));
        check_null((void *)meta_data[i].words_num_in_sentences);
        for(k = 0; k < meta_data[i].sentences_num; k++){
            meta_data[i].words_num_in_sentences[k] = 0;
        }
    }
    //get number of words in each sentence
    paragraph = sentence = character = 0;
    while(text[character] != '\0'){
        if( (text[character] == ' ') || (text[character] == '.') || (text[character] == '\n') ){
            meta_data[paragraph].words_num_in_sentences[sentence]++;
            if(text[character] == '.'){
                sentence++;
            }else if(text[character] == '\n'){
                paragraph++;
                sentence = 0;
            }
        }
        character++;
    }
    
    
    //allocate the main document pointer with first paragraph
    document = (char ****)malloc(MAX_PARAGRAPHS * sizeof(char ***));
    check_null((void *)document);
    
    //allocate paragrapgs pointers and sentences pointers
    for(paragraph = 0; i < MAX_PARAGRAPHS && meta_data[paragraph].sentences_num != 0; paragraph++){
        document[paragraph] = (char ***)malloc(meta_data[paragraph].sentences_num * sizeof(char **));
        check_null((void *)document[paragraph]);
        for(sentence = 0; sentence < meta_data[paragraph].sentences_num; sentence++){
            document[paragraph][sentence] = (char **)malloc(meta_data[paragraph].words_num_in_sentences[sentence] * sizeof(char));
            check_null((void *)document[paragraph][sentence]);
        }
    }
        
    //read document
    paragraph = sentence = word = character = 0; 
    for(i = 0; i < strlen(text); i++){
        if(text[i] != ' ' && text[i] != '.' && text[i] != '\n'){
            buffer[character++] = text[i]; 
        }else if(text[i] == ' '){
            buffer[character] = '\0';
            document[paragraph][sentence][word++] = strdup(buffer);
            character = 0;
        }else if(text[i] == '.'){
            buffer[character] = '\0';
            document[paragraph][sentence++][word] = strdup(buffer);
            word = character = 0;
        }else if(text[i] == '\n'){
            paragraph++;
            sentence = word = character = 0;
        }
    }
    return document;
}


char* get_input_text() {	
    int paragraph_count;
    scanf("%d", &paragraph_count);

    char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
    memset(doc, 0, sizeof(doc));
    getchar();
    for (int i = 0; i < paragraph_count; i++) {
        scanf("%[^\n]%*c", p[i]);
        strcat(doc, p[i]);
        if (i != paragraph_count - 1)
            strcat(doc, "\n");
    }

    char* returnDoc = (char*)malloc((strlen (doc)+1) * (sizeof(char)));
    strcpy(returnDoc, doc);
    return returnDoc;
}

void print_word(char* word) {
    printf("%s", word);
}

void print_sentence(char** sentence) {
    int word_count;
    scanf("%d", &word_count);
    for(int i = 0; i < word_count; i++){
        printf("%s", sentence[i]);
        if( i != word_count - 1)
            printf(" ");
    }
} 

void print_paragraph(char*** paragraph) {
    int sentence_count;
    scanf("%d", &sentence_count);
    for (int i = 0; i < sentence_count; i++) {
        print_sentence(*(paragraph + i));
        printf(".");
    }
}

int main() 
{
    char* text = get_input_text();
    char**** document = get_document(text);

    int q;
    scanf("%d", &q);

    while (q--) {
        int type;
        scanf("%d", &type);

        if (type == 3){
            int k, m, n;
            scanf("%d %d %d", &k, &m, &n);
            char* word = kth_word_in_mth_sentence_of_nth_paragraph(document, k, m, n);
            print_word(word);
        }

        else if (type == 2){
            int k, m;
            scanf("%d %d", &k, &m);
            char** sentence = kth_sentence_in_mth_paragraph(document, k, m);
            print_sentence(sentence);
        }

        else{
            int k;
            scanf("%d", &k);
            char*** paragraph = kth_paragraph(document, k);
            print_paragraph(paragraph);
        }
        printf("\n");
    }     
}