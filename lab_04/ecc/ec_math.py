import ecc.config as cfg



def invmod(k, p):
    # Extended Euclidean algorithm
    s, old_s = 0, 1
    r, old_r = p, k

    while r:
        quotient = old_r // r
        old_r, r = r, old_r - quotient * r
        old_s, s = s, old_s - quotient * s

    x = old_s  # (x * k) % p == 1
    return x % p


def add(point_1, point_2):
    if point_1 is None:
        return point_2
    if point_2 is None:
        return point_1

    x1, y1 = point_1
    x2, y2 = point_2

    # P + (-P) = 0
    if (x1 == x2) and (y1 != y2):
        return None

    # P1 = P2
    if x1 == x2:
        slope = (3 * x1 * x1 + 2 * cfg._CURVE_A * x1 + cfg._CURVE_B) * invmod(2 * y1, cfg._FIELD_MODULO)
    else:
        slope = (y1 - y2) * invmod(x1 - x2, cfg._FIELD_MODULO)

    x3 = (slope * slope - cfg._CURVE_A - x1 - x2) % cfg._FIELD_MODULO
    y3 = (y1 + slope * (x3 - x1))
    result = (x3, -y3 % cfg._FIELD_MODULO)

    return result


def scalar_mult(k, point):
    r0 = point
    r1 = add(point, point)
    t = k.bit_length()

    # Montgomery ladder
    for idx in range(t - 2, -1, -1):
        if (k >> idx) & 1:
            r0 = add(r0, r1)
            r1 = add(r1, r1)
        else:
            r1 = add(r0, r1)
            r0 = add(r0, r0)
    return r0



# a = 0x2ce10ff84f7abc8cf2ab9422cfdce8fbfdd9bc4568dc81131791bd52a4c848e1
# b = 0x5797dfb82b6e1b96ca1718c6ba516ab3e875f931419533ff9ea19621c13bfbfa
# p1 = (a, b)
# p2 = (b, a)
# # p3 = add(None, p1)
# p3 = scalar_mult(a, p1)
# print("\n####################")
# print(hex(p3[0]), hex(p3[1]), sep='\n')

# a = 0x7630498e5e4df030aedb1b0ea44ee1ce2a323427aaf2a959d9d31e39da843361 % cfg.SUBGROUP_ORDER
# print(hex(invmod(a, cfg.SUBGROUP_ORDER)))