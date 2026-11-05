<?php

declare(strict_types=1);

it('keeps a handle when another class boxes its address', function (): void {
    [$font, $buffer] = monoShaper('a');
    $address = $font->pointer();

    hb_buffer_t::fromPointer($address);

    expect($font->pointer())->toBe($address)
        ->and(hb_font_t::fromPointer($address))->toBe($font);
});

it('refuses a feature whose property was unset, instead of reading it as 0', function (): void {
    [$font, $buffer] = monoShaper('a');
    hb_feature_from_string('-kern', -1, $feature);
    unset($feature->value);

    expect(fn () => hb_shape($font, $buffer, [$feature], 1))
        ->toThrow(Error::class, 'must not be accessed before initialization');
});
