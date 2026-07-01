const API_BASE = import.meta.env.VITE_API_BASE ?? "";

export interface ShortenResult {
  code: string;
  long_url: string;
  short_url: string;
}

export interface LinkStats {
  code: string;
  long_url: string;
  clicks: number;
  created_at: string;
}

export class ApiError extends Error {
  status: number;
  constructor(status: number, message: string) {
    super(message);
    this.status = status;
  }
}

async function parseError(res: Response): Promise<never> {
  let message = `Request failed (${res.status})`;
  try {
    const body = await res.json();
    if (body?.error) message = body.error;
  } catch {
  }
  throw new ApiError(res.status, message);
}

export async function shorten(url: string, alias?: string): Promise<ShortenResult> {
  const res = await fetch(`${API_BASE}/api/shorten`, {
    method: "POST",
    headers: { "Content-Type": "application/json" },
    body: JSON.stringify(alias ? { url, alias } : { url }),
  });
  if (!res.ok) return parseError(res);
  return res.json();
}

export async function getStats(code: string): Promise<LinkStats> {
  const res = await fetch(`${API_BASE}/api/stats/${encodeURIComponent(code)}`);
  if (!res.ok) return parseError(res);
  return res.json();
}
