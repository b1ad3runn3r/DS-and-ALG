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
I suppose to solve this by creating a new file and writing there all present elements.
This resolves the problem of moving data chunks, but not the problem of shrinking the file.
