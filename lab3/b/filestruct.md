# File struct docs

## Brief description

This is an "official" documentation for file structure used in this project

## Structure

Here is a brief diagram of what file should look like:

Beginning of the file contains 2 main variables: `msize` and `csize`

```| msize || csize |```

Then it contain `csize` of series like this:

```| KeySpace || Item |```

The general file structure can be described like:

```| msize || csize || KeySpace || Item || KeySpace || Item | ...```

## KeySpace

KeySpace struct is implemented this way:

```C
struct KeySpace {
    int busy;
    long key_offset;
    long key_len;
    long par_offset;
    long par_len;
};
```

Then it is followed with Item structure immediately, so we don't
need to store offset of Item in KeySpace struct.

## Item

Item struct is quite simple and looks like this:

```C
struct Item {
    long offset;
};
```

## Real data

The real data can be accessed by offset and stored after the series.
Data is stored in random order, so we only need to know the offset and length:

```... | KeySpace || Item || data chunk || data chunk | ...```

## El problemo

The general problem with offsets is that they are not constant.
The main problem happens when we want to shrink the file or move some chunk(s) of data.
I suppose to solve it with some function that will iterate over file and change offset by some delta.
This delta can be determined from the size of data being (re)moved.
File `memmove()` analog (which is called `fmemmove()`) is quite simple, we just have to provide source offset and destination offset:

```C
int fmemmove(FILE *f, long off_src, long off_dst, size_t len);
```

What about the solution for the mentioned problem? I implemented this function:

```C
int change_offsets(Table *table, long delta);
```

It iterates over the file changing all the offsets on the right from the data by `delta` bytes.
