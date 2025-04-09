### Video Link for Demo
```
https://drive.google.com/file/d/1GWov7YhVZdSoALUURstcoI8xmBitrVmu/view?usp=drive_link
```

### Strategy
- `hft_client` detects simply momentum where the last 3 prices are stricly increasing or decreasing
- `deque` is used for O(1) complexity to discard old prices; this is achived by using `pop_front()`
- For each incoming price, `deque` will remove old prices if `size > 3` and append the incoming price. Then `hasMomentum` will be invoked to check for momentum.