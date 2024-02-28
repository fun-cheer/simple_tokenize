# simple_tokenize
simple tokenize code for Large Language Model(LLM) such as Llama
## vocabulary
vocab.bin is a binary file containing vocabulary with a typical size 32000, and it's struct is as below:
```
%d: vocabulary size
%d: max token length for this vocabulary
%d, %s: token length, token content
......
%d, %s: token length, token content
```
