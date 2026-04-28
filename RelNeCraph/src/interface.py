from sentence_transformers import SentenceTransformer
from typing import List, Dict, Any, Protocol, Tuple

class Model:
    def __init__(self,model_name = "BAAI/bge-base-en-v1.5"):
        self.Embedding_Model = SentenceTransformer(model_name)

class EmbeddingModel(Protocol):
    def encode(self, texts: List[str]) -> List[List[float]]:
        """Return normalized embeddings"""
        ...


class GraphStore(Protocol):
    def get_exact_node(self, term: str) -> Dict[str, Any]:
        """Exact lookup (indexed)"""
        ...

    def get_neighbors(self, node_id: str, depth: int, limit: int) -> List[Dict]:
        """1–2 hop expansion"""
        ...


class VectorStore(Protocol):
    def query(self, embedding: List[float], top_k: int) -> List[Tuple[Dict, float]]:
        """Return (node, score)"""
        ...


class Reranker(Protocol):
    def rerank(self, query: str, docs: List[str]) -> List[int]:
        """Return indices sorted by relevance"""
        ...


class LLM(Protocol):
    def generate(self, prompt: str, context: List[str]) -> str:
        """Final reasoning step"""
        ...