#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#define MAX_CHARACTERS 1005
#define MAX_PARAGRAPHS 5

struct word {
    char* data;
};

struct sentence {
    struct word* data;
    int word_count;//denotes number of words in a sentence
};

struct paragraph {
    struct sentence* data  ;
    int sentence_count;//denotes number of sentences in a paragraph
};

struct document {
    struct paragraph* data;
    int paragraph_count;//denotes number of paragraphs in a document
};

/*
    functions definitions and my structures
*/

/*
    Data structures and global variables
*/
FILE *fp; // Don't copy

typedef struct{
    int pharagraphs_num;
    int *sentences_num_in_ith_paragraph;
    int **words_num_in_ith_paragraph_and_jth_sentence;
}document_meta_data;

//utility functions
void *check_null(void *pointer){
    if(pointer == NULL){
        fprintf(stderr, "Memory allocation errror\n");
        exit(EXIT_FAILURE);
    }
    return pointer;
}

//Problem functions

struct document get_document(char* text) {
    document_meta_data *meta_data;
    struct document *doc = NULL;
    char buffer[MAX_CHARACTERS+1];
    int i, j, k, para_pos, sent_pos, word_pos;

    meta_data = (document_meta_data *)malloc(sizeof(document_meta_data));
    meta_data = (document_meta_data *)check_null((void *)meta_data);

    //getting number of paragraphs in text
    meta_data->pharagraphs_num = 1;
    for(i = 0; i < strlen(text); i++){
        if(text[i] == '\n'){
            meta_data->pharagraphs_num++;
        }
    }

    //getting sentences number in j-th paragraph
    meta_data->sentences_num_in_ith_paragraph = (int *)calloc(meta_data->pharagraphs_num, sizeof(int));
    meta_data->sentences_num_in_ith_paragraph = (int *)check_null((void *)meta_data->sentences_num_in_ith_paragraph);
    for(i = 0, j = 0; i < strlen(text) && j < meta_data->pharagraphs_num; i++){
        if(text[i] == '\n' || text[i] == '.'){
            if(text[i] == '\n'){
                j++;
            }else if(text[i] == '.'){
                meta_data->sentences_num_in_ith_paragraph[j]++;
            }
        }
    }

    //getting words number in j-th paragraph's k-th sentence
    meta_data->words_num_in_ith_paragraph_and_jth_sentence = (int **)calloc(meta_data->pharagraphs_num, sizeof(int *));
    meta_data->words_num_in_ith_paragraph_and_jth_sentence = (int **)check_null((void *)meta_data->words_num_in_ith_paragraph_and_jth_sentence);
    for(i = 0, j = 0; i < meta_data->pharagraphs_num && j < meta_data->pharagraphs_num; i++, j++){
        meta_data->words_num_in_ith_paragraph_and_jth_sentence[i] = (int *)calloc(meta_data->sentences_num_in_ith_paragraph[j], sizeof(int));
        meta_data->words_num_in_ith_paragraph_and_jth_sentence[i] = (int *)check_null((void *)meta_data->words_num_in_ith_paragraph_and_jth_sentence[i]);
    }
    for(i = 0, j = 0, k = 0; i < strlen(text); i++){
        if(text[i] == '\n'){
            j++;
            k = 0;
        }else if(text[i] == '.'){
            meta_data->words_num_in_ith_paragraph_and_jth_sentence[j][k]++;
            k++;
        }else if(text[i] == ' '){
            meta_data->words_num_in_ith_paragraph_and_jth_sentence[j][k]++;
        }
    }

    //allocate document structure with data collected
    doc = (struct document *)calloc(1, sizeof(struct document));
    doc = (struct document *)check_null((void *) doc);
    doc->paragraph_count = meta_data->pharagraphs_num;
    doc->data = (struct paragraph *)calloc(doc->paragraph_count, sizeof(struct paragraph));
    doc->data = (struct paragraph *)check_null((void *) doc->data);
    for(i = 0; i < doc->paragraph_count; i++){
        doc->data[i].sentence_count = meta_data->sentences_num_in_ith_paragraph[i];
        doc->data[i].data = (struct sentence *)calloc(doc->data[i].sentence_count, sizeof(struct sentence));
        doc->data[i].data = (struct sentence *)check_null((void *) doc->data[i].data);
        for(j = 0; j < doc->data[i].sentence_count; j++){
            doc->data[i].data[j].word_count = meta_data->words_num_in_ith_paragraph_and_jth_sentence[i][j];
            doc->data[i].data[j].data = (struct word *)calloc(doc->data[i].data[j].word_count, sizeof(struct word));
            doc->data[i].data[j].data = (struct word *)check_null((void *) doc->data[i].data[j].data);
        }
    }

    //read document
    para_pos = sent_pos = word_pos = 0;
    for(i = 0, k = 0; i < strlen(text); i++){
        if(text[i] == ' '){
            buffer[k] = '\0';
            //cpy word to its corresponding data structure position
            doc->data[para_pos].data[sent_pos].data[word_pos].data = strdup(buffer);
            doc->data[para_pos].data[sent_pos].data[word_pos].data = (char *)check_null((void *)doc->data[para_pos].data[sent_pos].data[word_pos].data);
            k = 0;
            word_pos++;
        }else if(text[i] == '.'){
            buffer[k] = '\0';
            doc->data[para_pos].data[sent_pos].data[word_pos].data = strdup(buffer);
            doc->data[para_pos].data[sent_pos].data[word_pos].data = (char *)check_null((void *)doc->data[para_pos].data[sent_pos].data[word_pos].data);
            sent_pos++;
            word_pos = 0;
            k = 0;
        }else if(text[i] == '\n'){
            para_pos++;
            sent_pos = 0;
            word_pos = 0;
        }else{
            buffer[k] = text[i];
            k++;
        }
    }

    //free memory
    
    return *doc;
}

struct word kth_word_in_mth_sentence_of_nth_paragraph(struct document Doc, int k, int m, int n) {
    return Doc.data[n-1].data[m-1].data[k-1];
}

struct sentence kth_sentence_in_mth_paragraph(struct document Doc, int k, int m) { 
    return Doc.data[m-1].data[k-1];
}

struct paragraph kth_paragraph(struct document Doc, int k) {
    return Doc.data[k-1];
}


/*
    Given functions
*/
void print_word(struct word w) {
    printf("%s", w.data);
}

void print_sentence(struct sentence sen) {
    for(int i = 0; i < sen.word_count; i++) {
        print_word(sen.data[i]);
        if (i != sen.word_count - 1) {
            printf(" ");
        }
    }
}

void print_paragraph(struct paragraph para) {
    for(int i = 0; i < para.sentence_count; i++){
        print_sentence(para.data[i]);
        printf(".");
    }
}

void print_document(struct document doc) {
    for(int i = 0; i < doc.paragraph_count; i++) {
        print_paragraph(doc.data[i]);
        if (i != doc.paragraph_count - 1)
            printf("\n");
    }
}

char* get_input_text() {	
    int paragraph_count;
    fscanf(fp, "%d%*c", &paragraph_count);

    char p[MAX_PARAGRAPHS][MAX_CHARACTERS], doc[MAX_CHARACTERS];
    memset(doc, 0, sizeof(doc));
    //getchar();
    for (int i = 0; i < paragraph_count; i++) {
        fscanf(fp, "%[^\n]%*c", p[i]);
        strcat(doc, p[i]);
        if (i != paragraph_count - 1)
            strcat(doc, "\n");
    }

    char* returnDoc = (char*)malloc((strlen (doc)+1) * (sizeof(char)));
    strcpy(returnDoc, doc);
    return returnDoc;
}

/*
    main
*/
int main() 
{

    fp = fopen("/home/axe47/C C++/Hacker_Rank/Structuring_the_Document/input.txt", "r");
    if (fp == NULL){
        printf("File error\n");
        exit(1);
    }

    char* text = get_input_text();
    struct document Doc = get_document(text);

    int q;
    fscanf(fp, "%d", &q);

    while (q--) {
        int type;
        fscanf(fp, "%d", &type);

        if (type == 3){
            int k, m, n;
            fscanf(fp, "%d %d %d", &k, &m, &n);
            struct word w = kth_word_in_mth_sentence_of_nth_paragraph(Doc, k, m, n);
            print_word(w);
        }

        else if (type == 2) {
            int k, m;
            fscanf(fp, "%d %d", &k, &m);
            struct sentence sen= kth_sentence_in_mth_paragraph(Doc, k, m);
            print_sentence(sen);
        }

        else{
            int k;
            fscanf(fp, "%d", &k);
            struct paragraph para = kth_paragraph(Doc, k);
            print_paragraph(para);
        }
        printf("\n");
    }
    fclose(fp);
    return 0;     
}