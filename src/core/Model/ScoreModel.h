#ifndef SCORE_MODEL_H
#define SCORE_MODEL_H
#include<string>
//成绩模型：定义学生成绩结构
//studentId->关联学生表StudentModel的Id字段
//courseId->关联CouresModle的Id字段
//teacherId->关联TeacherModel的Id字段

//绩点计算方法
//90 - 100分 → 4.0 
//85 - 89分 → 3.7
//82 - 84分 → 3.3
//78 - 81分 → 3.0
//75 - 77分 → 2.7
//72 - 74分 → 2.3
//68 - 71分 → 2.0
//64 - 67分 → 1.5
//60 - 63分 → 1.0
//60分以下 → 0.0

class ScoreModel
{
private:
    int id;                   //成绩Id，数据库自增
    int studentId;            //学生Id，关联学生表
    int courseId;             //课程Id，关联课程表
    double score;             //成绩
    std::string examTime;     //考试时间（格式XXXX-YY-MM）
    int teacherId;            //老师Id，关联老师表      

public:
    ScoreModel();
    //创建新成绩，无Id
    ScoreModel(int studentId, int courseId, double score, std::string examTime, int teacherId);
    //创建新成绩，有Id
    ScoreModel(int id, int studentId, int courseId, double score, std::string examTime, int teacherId);

    //Getter函数
    int getId() const;
    int getStudentId() const;
    int getCourseId() const;
    double getScore() const;
    std::string getExamTime() const;
    int getTeacherId() const;
    double getGpa() const;

    //Setter函数
    void setId(int id);
    void setStudentId(int studentId);
    void setCourseId(int courseId);
    void setScore(double score);
    void setExamTime(const std::string&examTime);
    void setTeacherId(int teacherId);

    //调试辅助函数
    std::string toString() const;
};
#endif