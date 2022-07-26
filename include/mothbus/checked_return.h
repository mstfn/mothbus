#pragma once

#define MOTH_CHECKED_RETURN(expr) { auto ec = expr; if (!!ec) return ec; }
