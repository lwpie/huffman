# Huffman

## Usage

```
encode --infile input --outfile output
decode --infile input --outfile output
```

## Speed

Tested on ```i5-8265U CPU @ 1.60GHz```, single core.

Use ```2018-11-13-raspbian-stretch-lite.img``` as example file.

```
λ encode --infile j --outfile d
0.00	Started
File Size: 1.74G
0.83	File Read
4.37	Char Counted
4.37	Tree Constructed
4.37	Tree Walked
20.25	File Encoded
20.44	Stopped
Overall Speed: 87.08M/s
Compression Rate: 47.68%
```

```
λ decode --infile d --outfile s
0.18	Started
0.19	Huffman Loaded
File Size: 848.76M
0.61	File Read
20.01	File Decoded
20.01	Stopped
Overall Speed: 88.94M/s
```

## To-dos

- Improve cli parameters parsing

- Support files greater than size_t